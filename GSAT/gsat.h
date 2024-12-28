#ifndef GSAT_H
#define GSAT_H

#include "../cnf.h"
#include <unordered_set>
#include <cstdlib>
#include <time.h>

class GSAT {
public:
    typedef std::vector<int> CLAUSE;
    typedef std::vector<CLAUSE> FORMULA;
    typedef std::vector<bool> MODEL;
    int maxSATclauses = 0;

    GSAT() {}
    MODEL run(FORMULA clauses, int symbolCount);
    MODEL run(FORMULA clauses, int symbolCount, int flipCount);
private:
    int maxFlips = 1024;
    int numSymbols = 0;

    MODEL GSAT_main(FORMULA& clauses);
    MODEL randAssignment(int numVars);
    int SATclauses(FORMULA& clauses, MODEL& assignment);
    MODEL flipSymbol(MODEL& assignment, int symbol);
};

GSAT::MODEL GSAT::run(FORMULA clauses, int symbolCount) {
    numSymbols = symbolCount;
    MODEL result = GSAT_main(clauses);
    if (maxSATclauses == clauses.size()) {
        return result;
    }
    return MODEL();
}

GSAT::MODEL GSAT::run(FORMULA clauses, int symbolCount, int flipCount) {
    srand(time(0));
    maxFlips = flipCount;
    numSymbols = symbolCount;
    MODEL result = GSAT_main(clauses);
    if (result.size() == clauses.size()) {
        return result;
    }
    return MODEL();
}

GSAT::MODEL GSAT::GSAT_main(FORMULA& clauses) {
    MODEL assignment = randAssignment(numSymbols);
    maxSATclauses = SATclauses(clauses, assignment);
    for (int i = 0; i < maxFlips; i++) {
        if (maxSATclauses == clauses.size()) {
            return assignment;
        }
        int idealsymbol = -1;
        int currMaxSATc = maxSATclauses;
        for (int j = 0; j < numSymbols; j++) {
            MODEL tempAssignment = flipSymbol(assignment, j);
            int tempMaxSATc = SATclauses(clauses, tempAssignment);
            if (tempMaxSATc > currMaxSATc) {
                currMaxSATc = tempMaxSATc;
                idealsymbol = j;
            }
        }
        if (idealsymbol != -1) {
            assignment = flipSymbol(assignment, idealsymbol);
            maxSATclauses = currMaxSATc;
        }
    }
    return assignment;
}

GSAT::MODEL GSAT::randAssignment(int numVars) {
    srand(time(0));
    MODEL assignment;
    for (int i = 0; i < numVars; i++) {
        assignment.push_back((bool) rand() % 2);
    }
    return assignment;
}

int GSAT::SATclauses(FORMULA& clauses, MODEL& assignment) {
    int MaxSATc = 0;
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            int assignmentIndex = std::abs(clauses[i][j]) - 1;
            bool symBool = assignment[assignmentIndex];
            if ((symBool && (clauses[i][j] > 0)) || (!symBool && (clauses[i][j] < 0))) {
                MaxSATc++;
                break;
            }
        }
    }
    return MaxSATc;
}

GSAT::MODEL GSAT::flipSymbol(MODEL& assignment, int symbol) {
    MODEL flippedSymbolAssignment = assignment;
    flippedSymbolAssignment[symbol] = !flippedSymbolAssignment[symbol];
    return flippedSymbolAssignment;
}

#endif