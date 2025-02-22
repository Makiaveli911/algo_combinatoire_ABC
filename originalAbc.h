#ifndef ORIGINAL_ABC_H
#define ORIGINAL_ABC_H


#include <vector>
#include <string>
#include <tuple>
#include <random>
#include "utils/Agent/Agent.h"
#include "utils/Problem/Problem.h"
#include "utils/Target/Target.h"

class OriginalABC {
public:
    /**
     * @brief Constructeur de l'algorithme ABC.
     * @param epoch Nombre total d'itérations de l'algorithme.
     * @param pop_size Taille de la population d'abeilles.
     * @param n_limits Nombre maximal d'échecs avant qu'une abeille employée devienne éclaireuse.
     */
    OriginalABC(int epoch = 10000, int pop_size = 100, int n_limits = 25);
    virtual ~OriginalABC();

    // initialisation des variables
    void initialize_variables() ;
    void initialization();
    void after_initialization();
    // Processus d'évolution des solutions
    void evolve(int epoch) ;
    void check_problem(Problem* problem);
    // Exécute l'algorithme
    Agent* solve(Problem* problem = nullptr);
    std::pair<std::vector<Agent*>, Agent*> update_global_best_agent(std::vector<Agent*>& pop, bool save = true);
    std::vector<double> correct_solution(const std::vector<double>& solution) const;
    static bool compare_target(const Target* target_x, const Target* target_y, const std::string& minmax = "min");
    int get_index_roulette_wheel_selection(const std::vector<double>& list_fitness);
    // Retourne le meilleur agent entre deux
    static Agent* get_better_agent(Agent* agent_x, Agent* agent_y, const std::string& minmax = "min", bool reverse = false);
    // Obtient les meilleurs et pires agents de la population
    static std::tuple<std::vector<Agent*>, std::vector<Agent*>, std::vector<Agent*>> get_special_agents(
        std::vector<Agent*>& pop, int n_best = 3, int n_worst = 3, const std::string& minmax = "min");
    static std::vector<Agent*> get_sorted_population(std::vector<Agent*>& pop);
    std::vector<Agent*> generate_population(int pop_size);
    Agent* generate_agent(const std::vector<double>& solution = {});
    Target* get_target(const std::vector<double>& solution, bool counted = true);

protected:
    int epoch;
    int pop_size;
    int n_limits;
    std::vector<int> trials; // Liste des échecs des solutions

    std::vector<Agent*> pop;
    Agent* g_best,*g_worst; // Meilleure et pire solution trouvées
    Problem* problem;

    // Historique des solutions
    std::vector<Agent> list_global_best;
    std::vector<Agent> list_current_best;
    std::vector<double> list_global_best_fit;
    std::vector<double> list_current_best_fit;
    std::vector<Agent> list_global_worst;
    std::vector<Agent> list_current_worst;
    std::vector<double> list_epoch_time;

    // Nombre d'évaluations de fitness
    int nfe_counter;
    // Générateur aléatoire
    std::mt19937 generator;

};

#endif // ORIGINAL_ABC_H