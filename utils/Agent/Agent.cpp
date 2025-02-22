#include "Agent.h"

Agent::Agent(const std::vector<double>& solution,
             const Target* target)
    : solution(solution),target(target) {}

std::vector<double> Agent::get_solution() const {
    return solution;
}
void Agent::set_target(Target* new_target) {
    this->target = new_target;
}
const Target *Agent::get_target() const {
    return target;
}
Agent Agent::copy() const {
    return Agent(this->solution, this->target);
}