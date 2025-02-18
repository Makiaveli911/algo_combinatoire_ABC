//
// Created by chaid on 11/02/2025.
//

#include "Problem.h"
#include <stdexcept>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <random>


// ---------------------- Constructeurs ----------------------

// Constructeur avec un seul bound
Problem::Problem(const std::vector<double>& lb,
                 const std::vector<double>& ub,
                 const std::string &minmax,
                 objective_function_t objective_function)
    : lb(lb), ub(ub),minmax(minmax), obj_func(objective_function), name("P"),
      n_objs(1), n_dims(0), save_population(false)
{
    set_functions();
}

// std::vector<FloatVar*> Problem::get_bounds() const {
//     return _bounds;
// }

// void Problem::set_bounds(const BaseVar* bounds) {
//     _bounds = bounds;
//     lb.clear();
//     ub.clear();
//     for (const auto& bound : bounds) {
//         lb.insert(lb.end(), bound->get_lb().begin(), bound->get_lb().end());
//         ub.insert(ub.end(), bound->get_ub().begin(), bound->get_ub().end());
//     }
// }
// void Problem::set_bounds(const FloatVar* bounds) {
//     std::vector<FloatVar*> bounds_list;
//     bounds_list.push_back(const_cast<FloatVar*>(bounds));
//     this->_bounds = bounds_list;
//     this->lb.clear();
//     this->ub.clear();
//     for (const auto& bound : this->_bounds) {
//         for (const auto& lb : bound->get_lb()) {
//             this->lb.push_back(lb);
//         }
//         for (const auto& ub : bound->get_ub()) {
//             this->ub.push_back(ub);
//         }
//     }
// }

// void Problem::set_seed(int seed) {
//     this->seed = seed;
//     for (auto& bound: _bounds) {
//         bound->set_seed(seed);
//     }
// }

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

