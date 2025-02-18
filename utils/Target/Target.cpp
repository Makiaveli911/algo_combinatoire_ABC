//
// Created by chaid on 10/02/2025.
//
#include "Target.h"

#include <numeric>
#include <stdexcept>

// // Fonction auxiliaire pour calculer le produit scalaire de deux vecteurs
// static double dot_product(const std::vector<double>& a, const std::vector<double>& b) {
//     if(a.size() != b.size()){
//         throw std::invalid_argument("Les vecteurs doivent avoir la même taille pour le produit scalaire.");
//     }
//     double sum = 0.0;
//     for (size_t i = 0; i < a.size(); ++i) {
//         sum += a[i] * b[i];
//     }
//     return sum;
// }

// Constructeur principal
Target::Target(const std::vector<double>& objectives, const std::vector<double>& weights) {

    set_objectives(objectives);
    set_weights(weights);
    calculate_fitness(this->_weights);
}
Target::Target(const double& objectives, const std::vector<double>& weights) {
    this->set_objectives(objectives);
    this->set_weights(weights);
    this->calculate_fitness(_weights);
}

void Target::set_objectives(const ObjectivesType& objs) {
    if (std::holds_alternative<double>(objs)) {
        this->_objectives = { std::get<double>(objs) };
    } else if (std::holds_alternative<std::vector<double>>(objs)) {
        this->_objectives = std::get<std::vector<double>>(objs);
    } else {
        throw std::invalid_argument("Invalid objectives. It should be a vector<double> or double.");
    }
}

// Méthode copy qui retourne une copie de l'objet
Target* Target::copy() const {
    Target* target = new Target(_objectives, _weights);
    return target;
}

// Getter pour objectives
std::vector<double> Target::objectives() const {
    return _objectives;
}

// Getter pour weights
std::vector<double> Target::weights() const {
    return _weights;
}

// Setter pour weights avec un vecteur
void Target::set_weights(const std::vector<double>& weights) {
    if(weights.empty()){
        _weights = std::vector<double>(_objectives.size(), 1.0);
    } else {
            _weights = weights;
        }
    }

// Getter pour fitness
double Target::fitness() const {
    return _fitness;
}

// Méthode pour calculer le fitness
void Target::calculate_fitness(const std::vector<double>& weights) {
    if(_weights.size() != _objectives.size()){
        throw std::invalid_argument("nombre de weight invalide ils correspondent pas");
    }
    _fitness = std::inner_product(_objectives.begin(),_objectives.end(), _weights.begin(), 0.0);
}
