#include "OriginalABC.h"

OriginalABC::OriginalABC(int epoch, int pop_size, int n_limits)
    : Optimizer(), n_limits(n_limits) {
    this->epoch = epoch;
    this->pop_size = pop_size;
}

OriginalABC::~OriginalABC() {}

void OriginalABC::initialize_variables() {
    trials.assign(pop_size, 0);
}
void OriginalABC::evolve(int epoch) {
    std::uniform_real_distribution<double> uniform_dist(-1.0, 1.0);
    std::uniform_int_distribution<int> int_dist(0, pop_size - 1);

    // Phase des abeilles employées
    for (int idx = 0; idx < pop_size; ++idx) {
        int rdx;
        do {
            rdx = int_dist(generator);
        } while (rdx == idx);

        std::vector<double> phi(problem->n_dims);
        for (auto& val : phi) val = uniform_dist(generator);

        std::vector<double> pos_new(problem->n_dims);
        for (size_t j = 0; j < problem->n_dims; ++j) {
            pos_new[j] = pop[idx]->solution[j] + phi[j] * (pop[rdx]->solution[j] - pop[idx]->solution[j]);
        }

        pos_new = correct_solution(pos_new);
        Agent* agent = generate_agent(pos_new);

        if (compare_target(agent->get_target(), pop[idx]->get_target(), problem->minmax)) {
            pop[idx] = agent;
            trials[idx] = 0;
        } else {
            trials[idx] += 1;
        }
    }

    // Phase des abeilles spectatrices
    std::vector<double> employed_fits;
    for (auto* agent : pop) {
        employed_fits.push_back(agent->target->fitness());
    }

    for (int idx = 0; idx < pop_size; ++idx) {
        int selected_bee = get_index_roulette_wheel_selection(employed_fits);
        int rdx;
        do {
            rdx = int_dist(generator);
        } while (rdx == idx || rdx == selected_bee);

        std::vector<double> phi(problem->n_dims);
        for (auto& val : phi) val = uniform_dist(generator);

        std::vector<double> pos_new(problem->n_dims);
        for (size_t j = 0; j < problem->n_dims; ++j) {
            pos_new[j] = pop[selected_bee]->solution[j] + phi[j] * (pop[rdx]->solution[j] - pop[selected_bee]->solution[j]);
        }

        pos_new = correct_solution(pos_new);
        Agent* agent = generate_agent(pos_new);

        if (compare_target(agent->get_target(), pop[selected_bee]->get_target(), problem->minmax)) {
            pop[selected_bee] = agent;
            trials[selected_bee] = 0;
        } else {
            trials[selected_bee] += 1;
        }
    }

    // Phase des abeilles éclaireuses (Scout Bees)
    for (int idx = 0; idx < pop_size; ++idx) {
        if (trials[idx] >= n_limits) {
            pop[idx] = generate_agent();
            trials[idx] = 0;
        }
    }
}

