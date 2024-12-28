#ifndef WALKSAT_H
#define WALKSAT_H

#include "../cnf.h"
#include <unordered_set>
#include <cstdlib>
#include <time.h>

class WalkSAT {
public:
    typedef std::vector<int> CLAUSE;
    typedef std::vector<CLAUSE> FORMULA;
    typedef std::vector<bool> MODEL;
    int maxSATclauses = 0;

    WalkSAT() {}
    MODEL run(FORMULA clauses, int symbolCount);
    MODEL run(FORMULA clauses, int symbolCount, int Prrob, int flipCount);
private:
    int maxFlips = 32767;
    int P = 50;
    int numSymbols = 0;

    MODEL WalkSAT_main(FORMULA& clauses);
    bool SATclause(CLAUSE& clause, MODEL& assignment);
    bool SATformula(FORMULA& clauses, MODEL& assignment);
    MODEL randAssignment(int numVars);
    CLAUSE randUNSATclause(FORMULA& clauses, MODEL& assignment);

};

WalkSAT::MODEL WalkSAT::run(FORMULA clauses, int symbolCount) {
    srand(time(0));
    numSymbols = symbolCount;
    MODEL result = WalkSAT_main(clauses);
    if (maxSATclauses == clauses.size()) {
        return result;
    }
    return MODEL();
}

WalkSAT::MODEL WalkSAT::run(FORMULA clauses, int symbolCount, int Prob, int flipCount) {
    srand(time(0));
    P = Prob;
    maxFlips = flipCount;
    numSymbols = symbolCount;
    MODEL result = WalkSAT_main(clauses);
    if (maxSATclauses == clauses.size()) {
        return result;
    }
    return MODEL();
}

WalkSAT::MODEL WalkSAT::WalkSAT_main(FORMULA& clauses) {
    MODEL assignment = randAssignment(numSymbols);
    for (int i = 0; i < maxFlips; i++) {
        if (SATformula(clauses, assignment)) {
            return assignment;
        }
        CLAUSE UNSATclause = randUNSATclause(clauses, assignment);
        if ((rand() % 101) < P) {
            int randLiteral = UNSATclause[rand() % UNSATclause.size()];
            int randSymbolIndex = std::abs(randLiteral) - 1;
            assignment[randSymbolIndex] = !assignment[randSymbolIndex];
        } else {
            int idealSymbol = -1;
            int currMaxSATc = -1;
            for (int j = 0; j < UNSATclause.size(); j++) {
                int flipSymbolIndex = std::abs(UNSATclause[j]) - 1;
                assignment[flipSymbolIndex] = !assignment[flipSymbolIndex];
                int tempMaxSATc = 0;
                for (int k = 0; k < clauses.size(); k++) {
                    if (SATclause(clauses[k], assignment)) {
                        tempMaxSATc++;
                    }
                }
                if (tempMaxSATc > currMaxSATc) {
                    currMaxSATc = tempMaxSATc;
                    if (currMaxSATc > maxSATclauses) {
                        maxSATclauses = currMaxSATc;
                    }
                    idealSymbol = flipSymbolIndex;
                }
                assignment[flipSymbolIndex] = !assignment[flipSymbolIndex];
            }
            assignment[idealSymbol] = !assignment[idealSymbol];
        }
    }
    return MODEL();
}

bool WalkSAT::SATformula(FORMULA& clauses, MODEL& assignment) {
    for (int i = 0; i < clauses.size(); i++) {
        if (!SATclause(clauses[i], assignment)) {
            return false;
        }
    }
    return true;
}

bool WalkSAT::SATclause(CLAUSE& clause, MODEL& assignment) {
    for (int i = 0; i < clause.size(); i++) {
        int assignmentIndex = std::abs(clause[i]) - 1;
        bool symBool = assignment[assignmentIndex];
        if ((symBool && (clause[i] > 0)) || (!symBool && (clause[i] < 0))) {
            return true;
        }
    }
    return false;
}

WalkSAT::MODEL WalkSAT::randAssignment(int numVars) {
    MODEL assignment;
    for (int i = 0; i < numVars; i++) {
        assignment.push_back((bool) rand() % 2);
    }
    return assignment;
}

WalkSAT::CLAUSE WalkSAT::randUNSATclause(FORMULA& clauses, MODEL& assignment) {
    FORMULA UNSATclauses;
    for (int i = 0; i < clauses.size(); i++) {
        if (!SATclause(clauses[i], assignment)) {
            UNSATclauses.push_back(clauses[i]);
        }
    }
    return UNSATclauses[rand() % UNSATclauses.size()];
}

#endif