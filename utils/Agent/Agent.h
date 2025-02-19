//
// Created by chaid on 11/02/2025.
//

#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../Target/Target.h"

class Agent {
public:
    // Identifiant unique auto-incrémenté pour chaque agent
    static int ID;

    // Constructeur
    // - solution : représente la position de l'agent (std::vector<double>)
    // - target : stocke la fitness de la solution (instance de Target)
    // - kwargs : paramètres supplémentaires
    Agent(const std::vector<double>& solution = std::vector<double>(),
          const Target* target = new Target(0.0));

    // Méthode de classe pour incrémenter l'identifiant
    static int increase();

    std::vector<double> get_solution() const;

    void set_target(Target* new_target);

    const Target *get_target() const;
    // Retourne une copie de l'agent
    Agent copy() const;

    // Attributs
    std::vector<double> solution;
    const Target* target;
    int id;
};

#endif // AGENT_H

