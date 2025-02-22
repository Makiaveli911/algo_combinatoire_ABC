#include "Problem.h"
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <random>

Problem::Problem(const std::vector<double>& lb,
                 const std::vector<double>& ub,
                 const std::string &minmax,
                 objective_function_t objective_function)
    : lb(lb), ub(ub),minmax(minmax), obj_func(objective_function), name("P"),
      n_objs(1), n_dims(0), save_population(false)
{
    set_functions();
}
void Problem::set_functions() {
    auto tested_solution = generate_solution();
    this->n_dims = tested_solution.size();
    this->n_objs = 1;
    this->obj_weights.resize(1, 1.0);
}
std::vector<double> Problem::generate_solution() const {
    std::vector<double> x_new(lb.size());
    std::random_device rd;
    std::mt19937 generator(rd());
        // Générer les valeurs directement dans la plage [lb, ub]
        for (size_t i = 0; i < lb.size(); ++i) {
            std::uniform_real_distribution<double> dist(lb[i], ub[i]);
            x_new[i] = dist(generator);
        }
    return x_new;
}
std::vector<double> Problem::correct_solution(const std::vector<double>& x) const {
    std::vector<double> x_new(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        x_new[i] = std::clamp(x[i], lb[i], ub[i]);
    }
    return x_new;
}
Target *Problem::get_target(const std::vector<double> &solution) {
    auto objs = obj_func(solution);
    return new Target(objs, obj_weights);
}