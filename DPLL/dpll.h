#ifndef DPLL_H
#define DPLL_H

#include "../cnf.h"
#include <unordered_set>

class DPLL {
public:
    DPLL() {}
    int maxCsatisfied = 0;
    std::pair<bool, std::unordered_set<int> > run(std::vector<std::vector<int> > clauses, std::array<int, 3> nums);

private:
    std::pair<bool, std::unordered_set<int> > DPLL_main(std::vector<std::vector<int> >& clauses, std::unordered_set<int>& model);
    int SATChecks(std::vector<std::vector<int> >& clauses);
    void unitProp(std::vector<std::vector<int> >& clauses, std::unordered_set<int>& model);
    std::vector<std::vector<int> > simplify(std::vector<std::vector<int> >& clauses, int literal);
    void PLE(std::vector<std::vector<int> >& clauses, std::unordered_set<int>& model);
    int pickLiteral(std::vector<std::vector<int> >& clauses);
};

std::pair<bool, std::unordered_set<int> > DPLL::run(std::vector<std::vector<int> > clauses, std::array<int, 3> nums) {
    std::unordered_set<int> model;
    return DPLL_main(clauses, model);
}

std::pair<bool, std::unordered_set<int> > DPLL::DPLL_main(std::vector<std::vector<int> >& clauses, std::unordered_set<int>& model) {
    int SATStat = SATChecks(clauses);
    if (SATStat != -1) {
        return std::make_pair(SATStat, model);
    }
    unitProp(clauses, model);
    PLE(clauses, model);
    SATStat = SATChecks(clauses);
    if (SATStat != -1) {
        return std::make_pair(SATStat, model);
    }
    int literal = pickLiteral(clauses);
    std::array<int, 2> litStates = {literal, -literal};
    for (int i = 0; i < 2; i++) {
        std::unordered_set<int> newModel = model;
        newModel.insert(litStates[i]);
        std::vector<std::vector<int> > newClauses = simplify(clauses, litStates[i]);
        std::pair<int, std::unordered_set<int> > branchDPLL = DPLL_main(newClauses, newModel);
        if (branchDPLL.first == 1) {
            return std::make_pair(true, branchDPLL.second);
        }
    }
    return std::make_pair(false, model);
} 

int DPLL::SATChecks(std::vector<std::vector<int> >& clauses) {
    if (clauses.size() ==0) {
        return 1;
    }
    for (int i = 0; i < clauses.size(); i++) {
        if (clauses[i].size() == 0) {
            return 0;
        }
    }
    return -1;
}

void DPLL::unitProp(std::vector<std::vector<int> >& clauses, std::unordered_set<int>& model) {
    bool unitClauseFound = true;
    while (unitClauseFound) {
        unitClauseFound = false;
        for (int i = 0; i < clauses.size(); i++) {
            if (clauses[i].size() == 1) {
                unitClauseFound = true;
                int literal = clauses[i][0];
                model.insert(literal);
                clauses = simplify(clauses, literal);
                break;
            }
        }
    }
}

std::vector<std::vector<int> > DPLL::simplify(std::vector<std::vector<int> >& clauses, int literal) {
    std::vector<std::vector<int> > simpClauses;
    int Csatisfied = 0;
    bool SATCFound = false;
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            if (clauses[i][j] == literal) {
                Csatisfied++;
                SATCFound = true;
                break;
            }
        }
        if (SATCFound) {
            SATCFound = false;
            continue;
        }
        std::vector<int> newClause;
        for (int j = 0; j < clauses[i].size(); j++) {
            if (clauses[i][j] != -literal) {
                newClause.push_back(clauses[i][j]);
            }
        }
        simpClauses.push_back(newClause);
    }
    if (Csatisfied > maxCsatisfied) {
        maxCsatisfied = Csatisfied;
    }
    return simpClauses;
}

void DPLL::PLE(std::vector<std::vector<int> >& clauses, std::unordered_set<int>& model) {
    std::unordered_map<int, int> litCounts;
    for (int i = 0; i < clauses.size(); i++) {
        std::vector<int> currClause = clauses[i];
        for (int j = 0; j < currClause.size(); j++) {
            litCounts[currClause[j]]++;
        }
    }
    std::vector<int> pureLits;
    for (auto it = litCounts.begin(); it != litCounts.end(); it++) {
        int literal = it->first;
        if (litCounts[literal] > 0 && litCounts[-literal] == 0) {
            pureLits.push_back(literal);
        }
    }
    for (int i = 0; i < pureLits.size(); i++) {
        model.insert(pureLits[i]);
        clauses = simplify(clauses, pureLits[i]);
    }
}

int DPLL::pickLiteral(std::vector<std::vector<int> >& clauses) {
    std::unordered_map<int, int> litCounts;
    for (int i = 0; i < clauses.size(); i++) {
        std::vector<int> currClause = clauses[i];
        for (int j = 0; j < currClause.size(); j++) {
            litCounts[currClause[j]]++;
        }
    }
    std::pair<int, int> maxLitCount = {0, 0};
    for (auto it = litCounts.begin(); it != litCounts.end(); it++) {
        int literal = it->first;
        if (litCounts[literal] > maxLitCount.second) {
            maxLitCount = {literal, litCounts[literal]};
        }
    }
    return maxLitCount.first;
}


#endif