#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <string>
#include <map>
#include "../Target/Target.h"
#include "../Problem/Problem.h"
#include <functional>

class Problem {
public:
    using objective_function_t = std::function<double(const std::vector<double>&)>;
    // Constructeurs
    Problem(const std::vector<double>& lb,
        const std::vector<double>& ub,
            const std::string &minmax = "min",
            objective_function_t objective_function = nullptr);

    //Destructeur
    virtual ~Problem() = default;

    std::vector<double> correct_solution(const std::vector<double>& x) const;
    std::vector<double>generate_solution() const;

    // Retourne l'objet Target associé à une solution donnée
    Target *get_target(const std::vector<double> &solution);

    // Variables membres
    std::vector<double> lb;
    std::vector<double> ub;
    std::string minmax;
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