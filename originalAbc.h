#ifndef ORIGINAL_ABC_H
#define ORIGINAL_ABC_H


#include <vector>
#include <string>
#include <map>
#include <variant>
#include <tuple>
#include <future>
#include <stdexcept>
#include <random>
#include "utils/Agent/Agent.h"
#include "utils/Problem/Problem.h"
#include "utils/Target/Target.h"

class OriginalABC {
public:
    OriginalABC(int epoch = 10000, int pop_size = 100, int n_limits = 25);
    virtual ~OriginalABC();

    void initialize_variables() ;
    void evolve(int epoch) ;
    void initialization();
    void after_initialization();
    void check_problem(Problem* problem);
    Agent* solve(Problem* problem = nullptr);
    std::pair<std::vector<Agent*>, Agent*> update_global_best_agent(std::vector<Agent*>& pop, bool save = true);
    std::vector<double> correct_solution(const std::vector<double>& solution) const;
    static bool compare_target(const Target* target_x, const Target* target_y, const std::string& minmax = "min");
    int get_index_roulette_wheel_selection(const std::vector<double>& list_fitness);
    static Agent* get_better_agent(Agent* agent_x, Agent* agent_y, const std::string& minmax = "min", bool reverse = false);
    static std::tuple<std::vector<Agent*>, std::vector<Agent*>, std::vector<Agent*>> get_special_agents(
        std::vector<Agent*>& pop, int n_best = 3, int n_worst = 3, const std::string& minmax = "min");
    static std::vector<Agent*> get_sorted_population(std::vector<Agent*>& pop, const std::string& minmax = "min", bool return_index = false);
    std::vector<Agent*> generate_population(int pop_size);
    Agent* generate_agent(const std::vector<double>& solution = {});
    // Agent* generate_empty_agent(const std::vector<double> &solution = {});
    Target* get_target(const std::vector<double>& solution, bool counted = true);
protected:
    int epoch;
    int pop_size;
    int n_limits;
    std::vector<int> trials;
    std::string mode;
    int n_workers;
    std::string name;
    std::vector<Agent*> pop;
    Agent* g_best,*g_worst;
    Problem* problem;
    // Historique des solutions
    std::vector<Agent> list_global_best;
    std::vector<Agent> list_current_best;
    std::vector<double> list_global_best_fit;
    std::vector<double> list_current_best_fit;
    std::vector<Agent> list_global_worst;
    std::vector<Agent> list_current_worst;
    std::vector<double> list_epoch_time;


    int nfe_counter;
    // Générateur aléatoire
    std::mt19937 generator;

};

#endif // ORIGINAL_ABC_H