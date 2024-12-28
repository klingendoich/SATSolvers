#include "../cnf.h"
#include "walksat.h"
#include <iostream>
#include <filesystem>
#include <chrono>


int main() {
    std::vector<std::string> formulaPaths;
    std::vector<std::string> formulas;
    namespace fs = std::filesystem;
    std::string path = "../PA3_Benchmarks/CNF Formulas/";
    std::string extension = ".cnf";

    // iterate through the folder
    for (auto it = fs::directory_iterator(path); it != fs::directory_iterator(); ++it) {
        if (it->is_regular_file() && it->path().extension() == extension) {
            formulaPaths.push_back(path + it->path().filename().string()); // store paths
            formulas.push_back(it->path().filename().string()); // store filenames
        }
    }
    path = "../PA3_Benchmarks/HARD CNF Formulas/";
    for (auto it = fs::directory_iterator(path); it != fs::directory_iterator(); ++it) {
        if (it->is_regular_file() && it->path().extension() == extension) {
            formulaPaths.push_back(path + it->path().filename().string());
            formulas.push_back(it->path().filename().string());
        }
    }
    for (int i = 0; i < 10; i++) {
    std::cout << "\n=======================================\n";
    std::cout <<   "         WalkSAT RUN-THROUGH #" << i << "\n";
    std::cout <<   "=======================================\n\n";
    for (int i = 0; i < formulaPaths.size(); i++) {
    //for (int i = 70; i < 71; i++) {
        std::cout << "-------------------{-------------------\n";
        std::cout << "| Formula file: " << formulas[i] << "\n";
        std::cout << "| -  -  -  -  -  -  -  -  -  -  -  -  -\n";

        std::array<int, 3> nums = getnums(formulaPaths[i]);
        std::pair<std::vector<std::vector<int> >, int> formula = readsentence(formulaPaths[i], nums);

        WalkSAT walksat;
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<bool> result = walksat.run(formula.first, nums[0]);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> secPassed = end - start;

        std::cout << "| Formula Condition: " << (!result.empty() ? "SAT" : "UNSAT") << "\n";
        std::cout << "| Max SAT Clauses: " << walksat.maxSATclauses << "\n";
        std::cout << "| Time Taken: " << secPassed.count() << " seconds\n";
        std::cout << "-------------------{-------------------\n";
        std::cout << "                   }                   \n";

    }
    }
    return 0;
}