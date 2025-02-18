// //
// // Created by chaid on 11/02/2025.
// //
//
// #ifndef FLOATVAR_H
// #define FLOATVAR_H
//
// // #include "../BaseVar/BaseVar.h"
// #include <vector>
// #include <stdexcept>
// #include <random>
//
// class FloatVar {
// public:
//
//     // Constructeur avec bornes vectorielles
//     FloatVar(const std::vector<double> &lb = {-10.0}, const std::vector<double> &ub={10.0}, const std::string &name = "float");
//
//     std::vector<double> encode(const std::vector<double> &x) const ;
//     std::vector<double> decode(const std::vector<double> &x) const ;
//     std::vector<double> correct(const std::vector<double> &x) const ;
//     std::vector<double> generate() const  ;
//
//     // Getters
//     std::vector<double> get_lb() const { return lb; }
//     std::vector<double> get_ub() const { return ub; }
//     int n_vars;
// private:
//     // Méthode privée pour définir les bounds (surchargée)
//     void _set_bounds(const std::vector<double> &lb, const std::vector<double> &ub);
//     // Variables membres
//     std::string name;
//
//     std::vector<double> lb;
//     std::vector<double> ub;
//     mutable std::mt19937 generator;
// };
//
// #endif // FLOATVAR_H
//
