#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include <variant>

class Target {
public:

    Target(const std::vector<double>& objectives= {},
           const std::vector<double>& weights = {});

    Target (const double& objectives = 0,
            const std::vector<double>& weights = {});

    using ObjectivesType = std::variant<std::vector<double>, double>;

    void set_objectives(const ObjectivesType& objs);

    // Getter pour weights
    std::vector<double> weights() const;
    // Getter pour objectives
    std::vector<double> objectives() const;

    // Getter pour fitness
    double fitness() const;

    // Setter pour weights
    void set_weights(const std::vector<double>& weights);

    // Méthode pour calculer le fitness
    void calculate_fitness(const std::vector<double>& weights);

private:
    std::vector<double> _objectives;
    std::vector<double> _weights;
    double _fitness;
};

#endif // TARGET_H