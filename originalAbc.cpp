#include "OriginalABC.h"
#include <chrono>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <future>
#include <cmath>
#include <functional>
#include <iostream>

OriginalABC::OriginalABC(int epoch, int pop_size, int n_limits)
    : n_limits(n_limits) {
    this->epoch = epoch;
    this->pop_size = pop_size;
}

OriginalABC::~OriginalABC() {}

void OriginalABC::initialize_variables() {
    trials.assign(pop_size, 0);
}
void OriginalABC::evolve(int epoch) {
    std::uniform_real_distribution<double> uniform_dist(-1.0, 1.0);
    std::uniform_int_distribution<int> int_dist(0, pop_size - 1);

    // Phase des abeilles employées
    for (int idx = 0; idx < pop_size; ++idx) {
        int rdx;
        do {
            rdx = int_dist(generator);
        } while (rdx == idx);

        std::vector<double> phi(problem->n_dims);
        for (auto& val : phi) val = uniform_dist(generator);

        std::vector<double> pos_new(problem->n_dims);
        for (size_t j = 0; j < problem->n_dims; ++j) {
            pos_new[j] = pop[idx]->solution[j] + phi[j] * (pop[rdx]->solution[j] - pop[idx]->solution[j]);
        }

        pos_new = correct_solution(pos_new);
        Agent* agent = generate_agent(pos_new);

        if (compare_target(agent->get_target(), pop[idx]->get_target(), problem->minmax)) {
            pop[idx] = agent;
            trials[idx] = 0;
        } else {
            trials[idx] += 1;
        }
    }

    // Phase des abeilles spectatrices
    std::vector<double> employed_fits;
    for (auto* agent : pop) {
        employed_fits.push_back(agent->target->fitness());
    }

    for (int idx = 0; idx < pop_size; ++idx) {
        int selected_bee = get_index_roulette_wheel_selection(employed_fits);
        int rdx;
        do {
            rdx = int_dist(generator);
        } while (rdx == idx || rdx == selected_bee);

        std::vector<double> phi(problem->n_dims);
        for (auto& val : phi) val = uniform_dist(generator);

        std::vector<double> pos_new(problem->n_dims);
        for (size_t j = 0; j < problem->n_dims; ++j) {
            pos_new[j] = pop[selected_bee]->solution[j] + phi[j] * (pop[rdx]->solution[j] - pop[selected_bee]->solution[j]);
        }

        pos_new = correct_solution(pos_new);
        Agent* agent = generate_agent(pos_new);

        if (compare_target(agent->get_target(), pop[selected_bee]->get_target(), problem->minmax)) {
            pop[selected_bee] = agent;
            trials[selected_bee] = 0;
        } else {
            trials[selected_bee] += 1;
        }
    }

    // Phase des abeilles éclaireuses (Scout Bees)
    for (int idx = 0; idx < pop_size; ++idx) {
        if (trials[idx] >= n_limits) {
            pop[idx] = generate_agent();
            trials[idx] = 0;
        }
    }
}

void OriginalABC::initialization(const std::vector<std::vector<double>>& starting_solutions ){
    pop.clear(); // Toujours vider la population avant d'initialiser

    if (!starting_solutions.empty() &&
        starting_solutions.size() == pop_size &&
        starting_solutions[0].size() == problem->n_dims) {
        // Utiliser les solutions fournies
        for (const auto& solution : starting_solutions) {
            pop.push_back(generate_agent(solution));
        }
        } else {
            // Générer une nouvelle population aléatoire
            pop = generate_population(pop_size);
        }
}

Agent* OriginalABC::generate_agent(const std::vector<double>& solution) {
    // Si la solution est vide, on en génère une nouvelle via `problem->generate_solution()`
    //sinon on l'utilise telle quelle
    std::vector<double> final_solution = solution.empty() ? problem->generate_solution() : solution;
    // Crée un nouvel Agent avec la solution et son Target associé
    return new Agent(final_solution, problem->get_target(final_solution));
}
Target* OriginalABC::get_target(const std::vector<double>& solution, bool counted) {
    if (counted) {
        nfe_counter++;
    }
    return problem->get_target(solution);
}
std::vector<Agent*> OriginalABC::generate_population(int pop_size) {
    pop_size = (pop_size > 0) ? pop_size : this->pop_size;
    std::vector<Agent*> pop(pop_size);

    for (auto& agent : pop) {
        agent = generate_agent();
    }
    return pop;
}

void OriginalABC::after_initialization() {
    auto [pop_temp, best, worst] = get_special_agents(pop, 1, 1, problem->minmax);
    g_best = best[0];
    g_worst = worst[0];

    // Stocker les meilleures et pires solutions initiales
    // store_initial_best_worst(*g_best, *g_worst);
    // Stocker les meilleures et pires solutions initiales directement ici
    list_global_best = {best[0]->copy()};
    list_current_best = {best[0]->copy()};
    list_global_best_fit = {best[0]->target->fitness()};
    list_current_best_fit = {best[0]->target->fitness()};

    list_global_worst = {worst[0]->copy()};
    list_current_worst = {worst[0]->copy()};
}

std::tuple<std::vector<Agent*>, std::vector<Agent*>, std::vector<Agent*>>
OriginalABC::get_special_agents(std::vector<Agent*>& pop, int n_best, int n_worst, const std::string& minmax) {
    std::vector<Agent*> sorted_pop = get_sorted_population(pop, minmax);

    std::vector<Agent*> best_agents, worst_agents;

    if (n_best > 0 && n_best <= static_cast<int>(sorted_pop.size())) {
        for (int i = 0; i < n_best; ++i) {
            best_agents.push_back(new Agent(*sorted_pop[i]));  // Copie des meilleurs agents
        }
    }
    if (n_worst > 0 && n_worst <= static_cast<int>(sorted_pop.size())) {
        for (int i = 0; i < n_worst; ++i) {
            worst_agents.push_back(new Agent(*sorted_pop[sorted_pop.size() - 1 - i]));  // Copie des pires agents
        }
    }
    return {sorted_pop, best_agents, worst_agents};
}

std::vector<Agent*> OriginalABC::get_sorted_population(std::vector<Agent*>& pop, const std::string& minmax, bool return_index) {
    std::vector<std::pair<double, Agent*>> fitness_pop;

    // Associer les agents à leurs valeurs de fitness
    for (Agent* agent : pop) {
        fitness_pop.emplace_back(agent->target->fitness(), agent);
    }

    // Trier la population en fonction du type de problème (minimisation ou maximisation)
    if (minmax == "max") {
        std::sort(fitness_pop.begin(), fitness_pop.end(), [](const auto& a, const auto& b) {
            return a.first > b.first;
        });
    } else {
        std::sort(fitness_pop.begin(), fitness_pop.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
    }

    // Extraire la population triée
    std::vector<Agent*> sorted_pop;
    std::vector<int> indices;
    for (size_t i = 0; i < fitness_pop.size(); ++i) {
        sorted_pop.push_back(fitness_pop[i].second);
        indices.push_back(static_cast<int>(i));
    }

    if (return_index) {
        return sorted_pop;  // Retourner uniquement les agents triés
    } else {
        return sorted_pop;
    }
}
void OriginalABC::check_problem(Problem* problem) {
    this->problem = problem;
    // generator.seed(seed);
    pop.clear();
    g_best = nullptr;
    g_worst = nullptr;
}

Agent* OriginalABC::solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions) {
    check_problem(problem);

    initialize_variables();
    initialization(starting_solutions);
    after_initialization();

    // Boucle principale d'évolution
    for (int epoch = 1; epoch <= this->epoch; ++epoch) {
        auto time_epoch_start = std::chrono::high_resolution_clock::now();
        evolve(epoch);

        // Mise à jour du meilleur agent global
        auto [pop_temp, best_agent] = update_global_best_agent(pop);
        g_best = best_agent;

        auto time_epoch_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_epoch = time_epoch_end - time_epoch_start;
        list_epoch_time.push_back(time_epoch.count());
    }

    // track_optimize_process();
    return g_best;
}

std::pair<std::vector<Agent*>, Agent*> OriginalABC::update_global_best_agent(std::vector<Agent*>& pop, bool save) {
    std::vector<Agent*> sorted_pop = get_sorted_population(pop, problem->minmax);
    Agent* c_best = sorted_pop.front();
    Agent* c_worst = sorted_pop.back();

    if (save) {
        // Sauvegarde du meilleur actuel
        list_current_best.push_back(*c_best);
        Agent* better = get_better_agent(c_best, &list_global_best.back(), problem->minmax);
        list_global_best.push_back(*better);

        // Sauvegarde du pire actuel
        list_current_worst.push_back(*c_worst);
        Agent* worse = get_better_agent(c_worst, &list_global_worst.back(), problem->minmax, true);
        list_global_worst.push_back(*worse);

        return {sorted_pop, better};
    } else {
        // Mise à jour du meilleur actuel
        Agent* local_better = get_better_agent(c_best, &list_current_best.back(), problem->minmax);
        list_current_best.back() = *local_better;
        Agent* global_better = get_better_agent(c_best, &list_global_best.back(), problem->minmax);
        list_global_best.back() = *global_better;

        // Mise à jour du pire actuel
        Agent* local_worst = get_better_agent(c_worst, &list_current_worst.back(), problem->minmax, true);
        list_current_worst.back() = *local_worst;
        Agent* global_worst = get_better_agent(c_worst, &list_global_worst.back(), problem->minmax, true);
        list_global_worst.back() = *global_worst;

        return {sorted_pop, global_better};
    }
}

Agent* OriginalABC::get_better_agent(Agent* agent_x, Agent* agent_y, const std::string& minmax, bool reverse) {
    std::map<std::string, int> minmax_dict = {{"min", 0}, {"max", 1}};
    int idx = minmax_dict[minmax];

    if (reverse) {
        idx = 1 - idx;
    }
    if (idx == 0) {
        return (agent_x->target->fitness() < agent_y->target->fitness()) ? new Agent(*agent_x) : new Agent(*agent_y);
    } else {
        return (agent_x->target->fitness() < agent_y->target->fitness()) ? new Agent(*agent_y) : new Agent(*agent_x);
    }
}

//Appelle la méthode correct_solution du problème pour ajuster la solution si nécessaire.
//Retour de la solution corrigée.
std::vector<double> OriginalABC::correct_solution(const std::vector<double>& solution) const {
    return problem->correct_solution(solution);
}
bool OriginalABC::compare_target(const Target* target_x, const Target* target_y, const std::string& minmax) {
    if (minmax == "min") {
        return target_x->fitness() < target_y->fitness();
    } else {
        return target_x->fitness() > target_y->fitness();
    }
}

int OriginalABC::get_index_roulette_wheel_selection(const std::vector<double>& list_fitness) {
    if (list_fitness.empty()) {
        throw std::invalid_argument("list_fitness cannot be empty.");
    }

    std::vector<double> adjusted_fitness = list_fitness;
    double min_fitness = *std::min_element(adjusted_fitness.begin(), adjusted_fitness.end());
    double max_fitness = *std::max_element(adjusted_fitness.begin(), adjusted_fitness.end());

    // Si tous les éléments sont égaux, retourner un index aléatoire
    if (min_fitness == max_fitness) {
        std::uniform_int_distribution<int> dist(0, adjusted_fitness.size() - 1);
        return dist(generator);
    }
    // Si des valeurs négatives existent, les ajuster
    if (min_fitness < 0) {
        for (auto& val : adjusted_fitness) {
            val -= min_fitness;
        }
    }
    // Inverser les valeurs si l'optimisation est en mode minimisation
    if (problem->minmax == "min") {
        for (auto& val : adjusted_fitness) {
            val = max_fitness - val;
        }
    }
    // Normaliser les probabilités
    double sum_fitness = std::accumulate(adjusted_fitness.begin(), adjusted_fitness.end(), 0.0);
    std::vector<double> probabilities;
    for (const auto& val : adjusted_fitness) {
        probabilities.push_back(val / sum_fitness);
    }
    // Sélection par roulette
    std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
    return distribution(generator);
}