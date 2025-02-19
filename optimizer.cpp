// //
// // Created by chaid on 11/02/2025.
// //
//
// #include "optimizer.h"
// #include <chrono>
// #include <numeric>
// #include <algorithm>
// #include <sstream>
// #include <future>
// #include <cmath>
// #include <functional>
// #include <iostream>
//
// // -------------------- Constructeur --------------------
// Optimizer::Optimizer()
//     : epoch(0), pop_size(0), mode("single"), n_workers(0), name(""), g_best(nullptr)
// , g_worst(nullptr), problem(nullptr), generator(std::random_device{}()) {}
//
// Optimizer::~Optimizer() {}
//
// // void Optimizer::store_initial_best_worst(const Agent& best, const Agent& worst) {
// //     // Stocker la meilleure solution initiale
// //     list_global_best.clear();
// //     list_current_best.clear();
// //     list_global_best_fit.clear();
// //     list_current_best_fit.clear();
// //
// //     list_global_best.push_back(best.copy());
// //     list_current_best.push_back(best.copy());
// //     list_global_best_fit.push_back(best.target->fitness());
// //     list_current_best_fit.push_back(best.target->fitness());
// //
// //     // Stocker la pire solution initiale
// //     list_global_worst.clear();
// //     list_current_worst.clear();
// //
// //     list_global_worst.push_back(worst.copy());
// //     list_current_worst.push_back(worst.copy());
// // }
//
// // std::string Optimizer::get_name() const {
// //     return name;
// // }
// void Optimizer::initialize_variables() {
//     // Implémentation minimale (ou complète selon vos besoins)
// }
// // void Optimizer::before_initialization(const std::vector<std::vector<double>>& starting_solutions) {
// //     if (starting_solutions.empty()) {
// //         return;
// //     } else if (starting_solutions.size() == pop_size && starting_solutions[0].size() == problem->n_dims) {
// //         pop.clear();
// //         for (const auto& solution : starting_solutions) {
// //             pop.push_back(generate_agent(solution));
// //         }
// //     } else {
// //         throw std::invalid_argument("Invalid starting_solutions. It should be a list of positions or a 2D matrix of positions only.");
// //     }
// // }
//
// void Optimizer::initialization(const std::vector<std::vector<double>>& starting_solutions ){
//     pop.clear(); // Toujours vider la population avant d'initialiser
//
//     if (!starting_solutions.empty() &&
//         starting_solutions.size() == pop_size &&
//         starting_solutions[0].size() == problem->n_dims) {
//         // Utiliser les solutions fournies
//         for (const auto& solution : starting_solutions) {
//             pop.push_back(generate_agent(solution));
//         }
//         } else {
//             // Générer une nouvelle population aléatoire
//             pop = generate_population(pop_size);
//         }
// }
//
// Agent* Optimizer::generate_agent(const std::vector<double>& solution) {
//     // Si la solution est vide, on en génère une nouvelle via `problem->generate_solution()`
//     //sinon on l'utilise telle quelle
//     std::vector<double> final_solution = solution.empty() ? problem->generate_solution() : solution;
//     // Crée un nouvel Agent avec la solution et son Target associé
//     return new Agent(final_solution, problem->get_target(final_solution));
// }
//
//
//
// // Agent* Optimizer::generate_empty_agent(const std::vector<double> &solution) {
// //
// //     // Si aucune solution n'est fournie, en générer une nouvelle
// //     if (solution.empty()) {
// //         final_solution = problem->generate_solution();
// //     }
// //
// //     return new Agent(final_solution);
// // }
// Target* Optimizer::get_target(const std::vector<double>& solution, bool counted) {
//     if (counted) {
//         nfe_counter++;
//     }
//     return problem->get_target(solution);
// }
//
// // std::vector<Agent*> Optimizer::generate_population(int pop_size) {
// //     if (pop_size <= 0) {
// //         pop_size = this->pop_size;
// //     }
// //     std::vector<Agent*> pop;
// //     for (int i = 0; i < pop_size; ++i) {
// //         pop.push_back(generate_agent());
// //     }
// //     return pop;
// // }
//
// //Évite un test if inutile en
// //j'ai remplacé if (pop_size <= 0) {...} par pop_size = (pop_size > 0) ? pop_size : this->pop_size;
// //std::vector<Agent*> pop(pop_size); pour réserver la mémoire en une seule allocation (plus efficace).
// std::vector<Agent*> Optimizer::generate_population(int pop_size) {
//     pop_size = (pop_size > 0) ? pop_size : this->pop_size;
//     std::vector<Agent*> pop(pop_size);
//
//     for (auto& agent : pop) {
//         agent = generate_agent();
//     }
//     return pop;
// }
//
//
// void Optimizer::after_initialization() {
//     auto [pop_temp, best, worst] = get_special_agents(pop, 1, 1, problem->minmax);
//     g_best = best[0];
//     g_worst = worst[0];
//
//     // Stocker les meilleures et pires solutions initiales
//     // store_initial_best_worst(*g_best, *g_worst);
//     // Stocker les meilleures et pires solutions initiales directement ici
//     list_global_best = {best[0]->copy()};
//     list_current_best = {best[0]->copy()};
//     list_global_best_fit = {best[0]->target->fitness()};
//     list_current_best_fit = {best[0]->target->fitness()};
//
//     list_global_worst = {worst[0]->copy()};
//     list_current_worst = {worst[0]->copy()};
// }
// std::tuple<std::vector<Agent*>, std::vector<Agent*>, std::vector<Agent*>>
// Optimizer::get_special_agents(std::vector<Agent*>& pop, int n_best, int n_worst, const std::string& minmax) {
//     std::vector<Agent*> sorted_pop = get_sorted_population(pop, minmax);
//
//     std::vector<Agent*> best_agents, worst_agents;
//
//     if (n_best > 0 && n_best <= static_cast<int>(sorted_pop.size())) {
//         for (int i = 0; i < n_best; ++i) {
//             best_agents.push_back(new Agent(*sorted_pop[i]));  // Copie des meilleurs agents
//         }
//     }
//     if (n_worst > 0 && n_worst <= static_cast<int>(sorted_pop.size())) {
//         for (int i = 0; i < n_worst; ++i) {
//             worst_agents.push_back(new Agent(*sorted_pop[sorted_pop.size() - 1 - i]));  // Copie des pires agents
//         }
//     }
//     return {sorted_pop, best_agents, worst_agents};
// }
//
// std::vector<Agent*> Optimizer::get_sorted_population(std::vector<Agent*>& pop, const std::string& minmax, bool return_index) {
//     std::vector<std::pair<double, Agent*>> fitness_pop;
//
//     // Associer les agents à leurs valeurs de fitness
//     for (Agent* agent : pop) {
//         fitness_pop.emplace_back(agent->target->fitness(), agent);
//     }
//
//     // Trier la population en fonction du type de problème (minimisation ou maximisation)
//     if (minmax == "max") {
//         std::sort(fitness_pop.begin(), fitness_pop.end(), [](const auto& a, const auto& b) {
//             return a.first > b.first;
//         });
//     } else {
//         std::sort(fitness_pop.begin(), fitness_pop.end(), [](const auto& a, const auto& b) {
//             return a.first < b.first;
//         });
//     }
//
//     // Extraire la population triée
//     std::vector<Agent*> sorted_pop;
//     std::vector<int> indices;
//     for (size_t i = 0; i < fitness_pop.size(); ++i) {
//         sorted_pop.push_back(fitness_pop[i].second);
//         indices.push_back(static_cast<int>(i));
//     }
//
//     if (return_index) {
//         return sorted_pop;  // Retourner uniquement les agents triés
//     } else {
//         return sorted_pop;
//     }
// }
// void Optimizer::check_problem(Problem* problem) {
//     this->problem = problem;
//     // generator.seed(seed);
//     pop.clear();
//     g_best = nullptr;
//     g_worst = nullptr;
// }
//
// Agent* Optimizer::solve(Problem* problem, const std::vector<std::vector<double>>& starting_solutions) {
//     check_problem(problem);
//
//     initialize_variables();
//     initialization(starting_solutions);
//     after_initialization();
//
//     // Boucle principale d'évolution
//     for (int epoch = 1; epoch <= this->epoch; ++epoch) {
//         auto time_epoch_start = std::chrono::high_resolution_clock::now();
//         evolve(epoch);
//
//         // Mise à jour du meilleur agent global
//         auto [pop_temp, best_agent] = update_global_best_agent(pop);
//         g_best = best_agent;
//
//         auto time_epoch_end = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> time_epoch = time_epoch_end - time_epoch_start;
//         list_epoch_time.push_back(time_epoch.count());
//     }
//
//     // track_optimize_process();
//     return g_best;
// }
//
// std::pair<std::vector<Agent*>, Agent*> Optimizer::update_global_best_agent(std::vector<Agent*>& pop, bool save) {
//     std::vector<Agent*> sorted_pop = get_sorted_population(pop, problem->minmax);
//     Agent* c_best = sorted_pop.front();
//     Agent* c_worst = sorted_pop.back();
//
//     if (save) {
//         // Sauvegarde du meilleur actuel
//         list_current_best.push_back(*c_best);
//         Agent* better = get_better_agent(c_best, &list_global_best.back(), problem->minmax);
//         list_global_best.push_back(*better);
//
//         // Sauvegarde du pire actuel
//         list_current_worst.push_back(*c_worst);
//         Agent* worse = get_better_agent(c_worst, &list_global_worst.back(), problem->minmax, true);
//         list_global_worst.push_back(*worse);
//
//         return {sorted_pop, better};
//     } else {
//         // Mise à jour du meilleur actuel
//         Agent* local_better = get_better_agent(c_best, &list_current_best.back(), problem->minmax);
//         list_current_best.back() = *local_better;
//         Agent* global_better = get_better_agent(c_best, &list_global_best.back(), problem->minmax);
//         list_global_best.back() = *global_better;
//
//         // Mise à jour du pire actuel
//         Agent* local_worst = get_better_agent(c_worst, &list_current_worst.back(), problem->minmax, true);
//         list_current_worst.back() = *local_worst;
//         Agent* global_worst = get_better_agent(c_worst, &list_global_worst.back(), problem->minmax, true);
//         list_global_worst.back() = *global_worst;
//
//         return {sorted_pop, global_better};
//     }
// }
//
// Agent* Optimizer::get_better_agent(Agent* agent_x, Agent* agent_y, const std::string& minmax, bool reverse) {
//     std::map<std::string, int> minmax_dict = {{"min", 0}, {"max", 1}};
//     int idx = minmax_dict[minmax];
//
//     if (reverse) {
//         idx = 1 - idx;
//     }
//     if (idx == 0) {
//         return (agent_x->target->fitness() < agent_y->target->fitness()) ? new Agent(*agent_x) : new Agent(*agent_y);
//     } else {
//         return (agent_x->target->fitness() < agent_y->target->fitness()) ? new Agent(*agent_y) : new Agent(*agent_x);
//     }
// }
//
// //Vérifie si chaque liste d'historique n'est pas vide.
// //Supprime le premier élément de chaque liste pour ne conserver que les données après la première itération.
// // void Optimizer::track_optimize_process() {
// //     if (!list_global_best.empty()) {
// //         list_global_best.erase(list_global_best.begin());
// //     }
// //     if (!list_current_best.empty()) {
// //         list_current_best.erase(list_current_best.begin());
// //     }
// //     if (!list_global_worst.empty()) {
// //         list_global_worst.erase(list_global_worst.begin());
// //     }
// //     if (!list_current_worst.empty()) {
// //         list_current_worst.erase(list_current_worst.begin());
// //     }
// // }
//
// //Appelle la méthode correct_solution du problème pour ajuster la solution si nécessaire.
// //Retour de la solution corrigée.
// std::vector<double> Optimizer::correct_solution(const std::vector<double>& solution) const {
//     return problem->correct_solution(solution);
// }
// bool Optimizer::compare_target(const Target* target_x, const Target* target_y, const std::string& minmax) {
//     if (minmax == "min") {
//         return target_x->fitness() < target_y->fitness();
//     } else {
//         return target_x->fitness() > target_y->fitness();
//     }
// }
//
// int Optimizer::get_index_roulette_wheel_selection(const std::vector<double>& list_fitness) {
//     if (list_fitness.empty()) {
//         throw std::invalid_argument("list_fitness cannot be empty.");
//     }
//
//     std::vector<double> adjusted_fitness = list_fitness;
//     double min_fitness = *std::min_element(adjusted_fitness.begin(), adjusted_fitness.end());
//     double max_fitness = *std::max_element(adjusted_fitness.begin(), adjusted_fitness.end());
//
//     // Si tous les éléments sont égaux, retourner un index aléatoire
//     if (min_fitness == max_fitness) {
//         std::uniform_int_distribution<int> dist(0, adjusted_fitness.size() - 1);
//         return dist(generator);
//     }
//     // Si des valeurs négatives existent, les ajuster
//     if (min_fitness < 0) {
//         for (auto& val : adjusted_fitness) {
//             val -= min_fitness;
//         }
//     }
//     // Inverser les valeurs si l'optimisation est en mode minimisation
//     if (problem->minmax == "min") {
//         for (auto& val : adjusted_fitness) {
//             val = max_fitness - val;
//         }
//     }
//     // Normaliser les probabilités
//     double sum_fitness = std::accumulate(adjusted_fitness.begin(), adjusted_fitness.end(), 0.0);
//     std::vector<double> probabilities;
//     for (const auto& val : adjusted_fitness) {
//         probabilities.push_back(val / sum_fitness);
//     }
//     // Sélection par roulette
//     std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
//     return distribution(generator);
// }