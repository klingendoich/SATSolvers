#include "../cnf.h"
#include "dpll.h"
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

    for (int i = 0; i < formulaPaths.size(); i++) {
    //for (int i = 5; i < 6; i++) {
        std::cout << "\n";
        std::cout << ",--------------------------------------\n";
        std::cout << "| Formula file: " << formulas[i] << "\n";
        std::cout << "| -  -  -  -  -  -  -  -  -  -  -  -  -\n";

        std::array<int, 3> nums = getnums(formulaPaths[i]);
        std::pair<std::vector<std::vector<int> >, int> formula = readsentence(formulaPaths[i], nums);

        DPLL dpll;
        auto start = std::chrono::high_resolution_clock::now();
        std::pair<bool, std::unordered_set<int> > result = dpll.run(formula.first, nums);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> secPassed = end - start;

        std::cout << "| Formula Condition: " << (result.first ? "SAT" : "UNSAT") << "\n";
        std::cout << "| Time Taken: " << secPassed.count() << " seconds\n";
        std::cout << "`--------------------------------------\n";
        if (result.first) {
            std::cout << "| Final Model: |\n";
            std::cout << "'--------------'\n";
            for (auto it = result.second.begin(); it != result.second.end(); it++) {
                std::cout << *it << ", ";
            }
            std::cout << std::endl;
        }//*/
        std::cout << "\n";

    }
    return 0;
}