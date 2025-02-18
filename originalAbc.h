#ifndef ORIGINAL_ABC_H
#define ORIGINAL_ABC_H

#include "Optimizer.h"
#include <vector>

class OriginalABC : public Optimizer {
public:
    OriginalABC(int epoch = 10000, int pop_size = 100, int n_limits = 25);
    virtual ~OriginalABC();

    void initialize_variables() override;
    void evolve(int epoch) override;

protected:
    int n_limits;
    std::vector<int> trials;
};

#endif // ORIGINAL_ABC_H