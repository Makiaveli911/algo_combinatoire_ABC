//
// Created by chaid on 11/02/2025.
//

#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <variant>
#include "../Space/BaseVar/BaseVar.h"
#include "../Space/FloatVar/FloatVar.h"
#include "../Target/Target.h"
#include "../Problem/Problem.h"
#include <functional>

class Problem {
public:
    using objective_function_t = std::function<double(const std::vector<double>&)>;
    // Constructeurs
    // Un seul bound (instance de BaseVar)
    Problem(const FloatVar* bounds,
            const std::string &minmax = "min",
            objective_function_t objective_function = nullptr);

    //Destructeur
    virtual ~Problem() = default;

    // Getter pour _bounds
    std::vector<FloatVar*> get_bounds() const;

    // Setters
    void set_bounds(const FloatVar* bounds);
    // void set_bounds(const std::vector<BaseVar*>& bounds);
    // void set_seed(int seed);


    std::vector<double> correct_solution(const std::vector<double>& x) const;
    std::vector<double>generate_solution() const;

    // Retourne l'objet Target associé à une solution donnée
    Target *get_target(const std::vector<double> &solution);

    // Variables membres
    std::vector<FloatVar*> _bounds;
    std::vector<double> lb;
    std::vector<double> ub;
    std::string minmax;
    // int seed;
    std::string name;
    int n_objs;
    int n_dims;
    bool save_population;
    std::string msg;
    std::vector<double> obj_weights;

private:
    objective_function_t obj_func;

    // Configure les fonctions en générant une solution test et en vérifiant obj_func
    void set_functions();




};

#endif // PROBLEM_H

