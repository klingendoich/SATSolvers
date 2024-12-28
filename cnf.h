#ifndef CNF_H
#define CNF_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

std::array<int, 3> getnums(std::string filename) {
    std::array<int, 3> nums;
    std::ifstream data(filename);
    std::istringstream current_line;
    std::string line;
    std::string segment;
    while (data.good()) {
        getline(data, line);
        if (line.length() != 0 && line[0] == 'p') {
            current_line.str(line);
            getline(current_line, segment, ' ');
            getline(current_line, segment, ' ');
            getline(current_line, segment, ' ');
            nums[0] = stoi(segment);
            getline(current_line, segment, ' ');
            getline(current_line, segment);
            nums[1] = stoi(segment);

            getline(data, line);
            int clauseSIZE = 0;
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {
                    clauseSIZE++;
                }
            }
            clauseSIZE--;
            nums[2] = clauseSIZE;
            break;
        }
    }
    data.close();
    return nums;
}

std::pair<std::vector<std::vector<int> >, int> readsentence(std::string filename, std::array<int, 3> nums) {
    std::vector<std::vector<int> > sentence;
    int numVARS = nums[0];
    int clauseSIZE = nums[2];
    std::ifstream data(filename);
    std::istringstream current_line;
    std::string line;
    std::string segment;

    getline(data, line);
    while (data.good() && line[0] == 'c') {
        getline(data, line);
        if (line[0] == 'p') {
            break;
        }
    }

    std::vector<int> literals;
    while (data.good()) {
        getline(data, line);
        if (line.length() == 0 || line[0] == '%' || line[0] == 'c') {
            break;
        }
        current_line.str(line);
        if (current_line.peek() == ' ') {
            getline(current_line, segment, ' ');
        }
        for (int i = 0; i < clauseSIZE; i++) {
            getline(current_line, segment, ' ');
            literals.push_back(stoi(segment));
        }
        sentence.push_back(literals);
        literals.clear();
    }


    data.close();
    return std::make_pair(sentence, numVARS);
}

#endif