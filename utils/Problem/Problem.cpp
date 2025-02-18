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
Problem::Problem(const FloatVar* bounds,
                 const std::string &minmax,
                 objective_function_t objective_function)
    : minmax(minmax), obj_func(objective_function), name("P"),
      n_objs(1), n_dims(0), save_population(false)
{
    set_bounds(bounds);
    set_functions();
}

std::vector<FloatVar*> Problem::get_bounds() const {
    return _bounds;
}

// void Problem::set_bounds(const BaseVar* bounds) {
//     _bounds = bounds;
//     lb.clear();
//     ub.clear();
//     for (const auto& bound : bounds) {
//         lb.insert(lb.end(), bound->get_lb().begin(), bound->get_lb().end());
//         ub.insert(ub.end(), bound->get_ub().begin(), bound->get_ub().end());
//     }
// }
void Problem::set_bounds(const FloatVar* bounds) {
    std::vector<FloatVar*> bounds_list;
    bounds_list.push_back(const_cast<FloatVar*>(bounds));
    this->_bounds = bounds_list;
    this->lb.clear();
    this->ub.clear();
    for (const auto& bound : this->_bounds) {
        for (const auto& lb : bound->get_lb()) {
            this->lb.push_back(lb);
        }
        for (const auto& ub : bound->get_ub()) {
            this->ub.push_back(ub);
        }
    }
}


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
    std::vector<std::vector<double>> x;
    for (const auto& var : this->_bounds) {
        auto generated = var->generate();
        x.push_back(generated);
    }
    std::vector<double> x_new;
    for (size_t i = 0; i < this->_bounds.size(); ++i) {
        auto encoded = this->_bounds[i]->encode(std::vector<double>{x[i]});
        x_new.insert(x_new.end(), encoded.begin(), encoded.end());
    }
    return x_new;
}
std::vector<double> Problem::correct_solution(const std::vector<double>& x) const {
    std::vector<double> x_new;
    size_t n_vars = 0;
    for (size_t i = 0; i < this->_bounds.size(); ++i) {
        auto corrected = this->_bounds[i]->correct(std::vector<double>(x.begin() + n_vars, x.begin() + n_vars + this->_bounds[i]->n_vars));
        x_new.insert(x_new.end(), corrected.begin(), corrected.end());
        n_vars += this->_bounds[i]->n_vars;
    }
    return x_new;
}

Target *Problem::get_target(const std::vector<double> &solution) {
    auto objs = obj_func(solution);
    return new Target(objs, obj_weights);
}

