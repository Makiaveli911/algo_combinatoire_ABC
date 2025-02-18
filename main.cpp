#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include "utils/Space/FloatVar/FloatVar.h"
#include "OriginalABC.h"
#include "utils/Problem/Problem.h"
#include "utils/Agent/Agent.h"
#include "utils/Target/Target.h"

// --- La fonction objective_function telle que dans le script Python ---
double objective_function(const std::vector<double>& solution) {
    double sum = 0.0;
    for (double x : solution) {
        sum += x * x;
    }
    return sum;
}


struct ProblemDict {
    std::vector<double>& lb;
    std::vector<double>& ub;
    std::string minmax;
    std::function<double(const std::vector<double>&)> obj_func;
};

int main() {
    std::vector<double> lb(30,-10);
    std::vector<double> ub(30,10);
    // --- Création du problem_dict exactement comme dans le script Python ---
    Problem* problem= new Problem(lb,ub,"min",objective_function);

    // --- Instanciation du modèle OriginalABC avec epoch=1000, pop_size=50, n_limits=50 ---
    OriginalABC model(1000, 50, 50);

    // --- Conversion du problem_dict en un objet Problem via MyProblem ---
    Agent* g_best = model.solve(problem);

    // --- Affichage de la solution et de la fitness (g_best correspond à la variable Python g_best) ---
    std::cout << "Solution: ";
    for (double v : g_best->solution) {
        std::cout << v << " ";
    }
    std::cout << ", Fitness: " << g_best->target->fitness() << std::endl;

    return 0;
}
