#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include "../Target/Target.h"

class Agent {
public:
    // Constructeur
    // - solution : représente la position de l'agent (std::vector<double>)
    // - target : stocke la fitness de la solution (instance de Target)
    Agent(const std::vector<double>& solution = std::vector<double>(),
          const Target* target = new Target(0.0));

    std::vector<double> get_solution() const;
    void set_target(Target* new_target);
    const Target *get_target() const;
    // Retourne une copie de l'agent
    Agent copy() const;

    std::vector<double> solution;
    const Target* target;
};

#endif // AGENT_H

