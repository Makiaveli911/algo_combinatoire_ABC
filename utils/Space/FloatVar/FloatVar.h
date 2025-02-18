//
// Created by chaid on 11/02/2025.
//

#ifndef FLOATVAR_H
#define FLOATVAR_H

#include "../BaseVar/BaseVar.h"
#include <vector>
#include <stdexcept>
#include <random>

class FloatVar : public BaseVar {
public:

    // Constructeur avec bornes vectorielles
    FloatVar(const std::vector<double> &lb = {-10.0}, const std::vector<double> &ub={10.0}, const std::string &name = "float");

    std::vector<double> encode(const std::vector<double> &x) const override;
    std::vector<double> decode(const std::vector<double> &x) const override;
    std::vector<double> correct(const std::vector<double> &x) const override;
    std::vector<double> generate() const override ;

private:
    // Méthode privée pour définir les bounds (surchargée)
    void _set_bounds(const std::vector<double> &lb, const std::vector<double> &ub);
};

#endif // FLOATVAR_H

