//
// Created by chaid on 11/02/2025.
//

#include "Agent.h"

// Initialisation de l'identifiant statique
int Agent::ID = 0;

Agent::Agent(const std::vector<double>& solution,
             const Target* target)
    : solution(solution),target(target),id(increase()) {}

int Agent::increase() {
    ID += 1;
    return ID;
}

std::vector<double> Agent::get_solution() const {
    return solution;
}

void Agent::set_target(Target* new_target) {
    this->target = new_target;
}

const Target *Agent::get_target() const {
    // Assurez-vous que l'objet target existe dans Agent
    return target;  // 'target' doit être un membre de Agent
}

Agent Agent::copy() const {
    // Crée une nouvelle instance d'Agent en copiant solution, target et kwargs.
    // Puisque target est stocké par valeur, le constructeur de copie de Target sera utilisé.
    return Agent(this->solution, this->target);
}

