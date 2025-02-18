// //
// // Created by chaid on 11/02/2025.
// //
//
// #include "FloatVar.h"
// #include <algorithm> // pour std::clamp (C++17)
//
// #include <cmath>
// #include <algorithm>
//
// FloatVar::FloatVar(const std::vector<double> &lb, const std::vector<double> &ub, const std::string &name)
//     : name(name), n_vars(0), lb(lb), ub(ub)
// {
//     n_vars = static_cast<int>(lb.size());
//     std::random_device random;
//     generator = std::mt19937(random());
// }

// void FloatVar::_set_bounds(const std::vector<double> &lb, const std::vector<double> &ub) {
//     if (lb.size() != ub.size()) {
//         throw std::invalid_argument("Invalid lb or ub. Length of lb should equal to length of ub.");
//     }
//     this->lb = lb;
//     this->ub = ub;
//     n_vars = static_cast<int>(lb.size());
// }

// std::vector<double> FloatVar::encode(const std::vector<double> &x) const {
//     // Retourne x converti en vecteur de double (déjà le cas)
//     return x;
// }

// std::vector<double> FloatVar::decode(const std::vector<double> &x) const {
//     // Applique correct sur x puis retourne le résultat
//     return correct(x);
// }

// std::vector<double> FloatVar::correct(const std::vector<double> &x) const {
//     if (x.size() != lb.size()) {
//         throw std::invalid_argument("Input vector size does not match bounds size.");
//     }
//     std::vector<double> result(x.size());
//     for (size_t i = 0; i < x.size(); ++i) {
//         // std::clamp pour contraindre x[i] entre lb[i] et ub[i]
//         result[i] = std::clamp(x[i], lb[i], ub[i]);
//     }
//     return result;
// }
//
// std::vector<double> FloatVar::generate() const{
//     std::vector<double> result(n_vars);
//     // static std::mt19937 global_generator(std::random_device{}());
//     for (int i = 0; i < n_vars; ++i) {
//         std::uniform_real_distribution<double> dist(lb[i], ub[i]);
//         // double rnd = rand()/RAND_MAX;
//         // result[i] =lb[i] + rnd * (ub[i] - lb[i]);
//         result[i] = dist(generator);
//     }
//     return result;
// }

