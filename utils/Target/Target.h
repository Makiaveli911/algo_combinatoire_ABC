#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include <stdexcept>
#include <variant>

class Target {
public:

    // Constructeur principal : prend en paramètre un vecteur d'objectives et éventuellement un vecteur de weights.
    Target(const std::vector<double>& objectives= {},
           const std::vector<double>& weights = {});

    Target (const double& objectives = 0,
            const std::vector<double>& weights = {});

    // Méthode copy qui retourne une copie de l'objet
    Target* copy() const;

    // Getter pour objectives
    std::vector<double> objectives() const;

    using ObjectivesType = std::variant<std::vector<double>, double>;

    void set_objectives(const ObjectivesType& objs);

    // Getter pour weights
    std::vector<double> weights() const;

    // Getter pour fitness
    double fitness() const;

    // Setter pour weights (surcharge)
    void set_weights(const std::vector<double>& weights);

    // Méthode pour calculer le fitness
    void calculate_fitness(const std::vector<double>& weights);

private:
    // Variables membres conservant exactement le même nom que dans votre code Python
    std::vector<double> _objectives;
    std::vector<double> _weights;
    double _fitness;
};

#endif // TARGET_H