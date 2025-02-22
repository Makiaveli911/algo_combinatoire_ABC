// #include <iostream>
// #include <vector>
// #include <memory>
// #include <functional>
// #include "OriginalABC.h"
// #include "utils/Problem/Problem.h"
// #include "utils/Agent/Agent.h"
// #include "utils/Target/Target.h"
//
// // Cette fonction calcule la somme des carrés des éléments d'une solution.
// // Plus cette somme est faible, meilleure est la solution (problème de minimisation).
// double objective_function(const std::vector<double>& solution) {
//     double sum = 0.0;
//     for (double x : solution) {
//         sum += x * x;
//     }
//     return sum;
// }
//
// struct ProblemDict {
//     std::vector<double>& lb; // Bornes inférieures
//     std::vector<double>& ub;// Bornes supérieures
//     std::string minmax;
//     std::function<double(const std::vector<double>&)> obj_func;
// };
//
// int main() {
//     // Définition des bornes inférieures et supérieures pour une dimension de 30
//     std::vector<double> lb(30,-10);
//     std::vector<double> ub(30,10);
//     // Création du problème avec les bornes et la fonction objectif
//     Problem* problem= new Problem(lb,ub,"min",objective_function);
//
//     // Instanciation du modèle OriginalABC avec epoch=1000, pop_size=50, n_limits=50
//     OriginalABC model(1000, 50, 50);
//
//     // Lancement de l'algorithme ABC
//     // La méthode solve() exécute l'algorithme et retourne la meilleure solution trouvée.
//     Agent* g_best = model.solve(problem);
//
//     // Affichage des resultat
//     std::cout << "Solution: ";
//     for (double v : g_best->solution) {
//         std::cout << v << " ";
//     }
//     std::cout << ", Fitness: " << g_best->target->fitness() << std::endl;
//
//     return 0;
// }
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <fstream>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <numeric>
#include "OriginalABC.h"
#include "utils/Problem/Problem.h"
#include "utils/Agent/Agent.h"
#include "utils/Target/Target.h"

// Définition des benchmarks
double rosenbrock_function(const std::vector<double>& solution) {
    double sum = 0.0;
    for (size_t i = 0; i < solution.size() - 1; ++i) {
        sum += std::pow(1.0 - solution[i], 2.0) + 100.0 * std::pow(solution[i + 1] - solution[i] * solution[i], 2.0);
    }
    return sum;
}

double rastrigin_function(const std::vector<double>& solution) {
    double A = 10.0;
    double sum = A * solution.size();
    for (double x : solution) {
        sum += x * x - A * std::cos(2.0 * M_PI * x);
    }
    return sum;
}

double ackley_function(const std::vector<double>& solution) {
    double a = 20.0, b = 0.2, c = 2 * M_PI;
    size_t d = solution.size();
    double sum1 = 0.0, sum2 = 0.0;

    for (double x : solution) {
        sum1 += x * x;
        sum2 += std::cos(c * x);
    }

    return -a * std::exp(-b * std::sqrt(sum1 / d)) - std::exp(sum2 / d) + a + std::exp(1);
}

// Structure pour gérer les benchmarks
struct ProblemDict {
    std::vector<double> lb;
    std::vector<double> ub;
    std::string minmax;
    std::function<double(const std::vector<double>&)> obj_func;
};

int main() {
    std::cout << "Demarrage du programme...\n";
    // Paramètres de validation
    std::vector<std::pair<std::string, std::function<double(const std::vector<double>&)>>> functions = {
        {"rosenbrock", rosenbrock_function},
        {"rastrigin", rastrigin_function},
        {"ackley", ackley_function}
    };
    std::vector<int> dimensions = {30, 50, 100};
    int num_executions = 10;
    int pop_size = 30;
    int epochs = 5000;

    // Nettoyage du dossier "results"
    std::filesystem::path results_path = "results";
    std::cout << "Les resultats seront enregistres dans : " << results_path << std::endl;
    if (std::filesystem::exists(results_path)) {
        std::cout << "Nettoyage du dossier results...\n";
        for (const auto& entry : std::filesystem::directory_iterator(results_path)) {
            if (entry.path().extension() == ".txt") {
                std::filesystem::remove(entry.path());
            }
        }
    } else {
        std::filesystem::create_directory(results_path);
    }

    // Mesure du temps d'exécution total
    auto start_time = std::chrono::high_resolution_clock::now();

    for (const auto& [func_name, func] : functions) {
        std::cout << "Traitement de la fonction : " << func_name << "\n";

        for (int dim : dimensions) {
            std::cout << "  - Dimension : " << dim << "\n";
            std::vector<double> results;
            std::string filename = "results/results_" + func_name + "_" + std::to_string(dim) + ".txt";
            std::ofstream file(filename);

            for (int i = 0; i < num_executions; ++i) {
                auto exec_start_time = std::chrono::high_resolution_clock::now();
                std::cout << "    > Execution " << (i + 1) << "/" << num_executions << " en cours...\n";

                // Définition des bornes
                std::vector<double> lb(dim, -10.0);
                std::vector<double> ub(dim, 10.0);

                // Création du problème
                Problem problem(lb, ub, "min", func);

                // Exécution de l'algorithme OriginalABC
                OriginalABC model(epochs, pop_size, 50);
                Agent* g_best = model.solve(&problem);

                // Stockage du fitness
                double fitness = g_best->target->fitness();
                results.push_back(fitness);

                auto exec_end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> exec_elapsed_time = exec_end_time - exec_start_time;

                std::cout << "    > Execution " << (i + 1) << " terminee. Fitness: " << fitness
                          << " (Duree: " << exec_elapsed_time.count() << " sec)\n";

                file << "Fonction: " << func_name << ", Dimension: " << dim
                     << ", Execution: " << (i + 1) << ", Fitness: " << fitness << "\n";
            }

            // Calcul des statistiques (moyenne et écart-type)
            double avg_fitness = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
            double sum_sq = std::inner_product(results.begin(), results.end(), results.begin(), 0.0);
            double variance = (sum_sq / results.size()) - (avg_fitness * avg_fitness);
            double std_fitness = std::sqrt(variance);

            file << "Fonction: " << func_name << ", Dimension: " << dim
                 << ", Fitness Moyenne sur " << num_executions << " executions: " << avg_fitness << "\n";
            file << "Fonction: " << func_name << ", Dimension: " << dim
                 << ", Ecart Type sur " << num_executions << " executions: " << std_fitness << "\n";

            file.close();
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Temps total d'execution: " << elapsed_time.count() << " secondes\n";
    std::cout << "Fin du programme.\n";

    return 0;
}
