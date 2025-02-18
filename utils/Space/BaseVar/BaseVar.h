//
// Created by chaid on 11/02/2025.
//

#ifndef BASEVAR_H
#define BASEVAR_H

#include <vector>
#include <random>
#include <string>
#include <stdexcept>

class BaseVar {
public:
    // Constructeur avec nom par défaut "variable"
    BaseVar(const std::string &name = "variable");

    //destructeur
    virtual ~BaseVar() = default;

    // Getter et setter pour la propriété seed
    // int seed() const;
    // void set_seed(int value);
    std::vector<double> get_lb() const { return lb; }
    std::vector<double> get_ub() const { return ub; }

    // Méthodes abstraites (simulant les méthodes @abstractmethod de Python)
    virtual std::vector<double> encode(const std::vector<double> &x) const = 0;
    virtual std::vector<double> decode(const std::vector<double> &x) const = 0;
    virtual std::vector<double> correct(const std::vector<double> &x) const = 0;
    virtual std::vector<double> generate() const = 0;

    // Variables membres
    std::string name;
    int n_vars;
    std::vector<double> lb;
    std::vector<double> ub;

protected:
    // int _seed;
    mutable std::mt19937 generator ;
};

#endif // BASEVAR_H
