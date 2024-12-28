# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -fsanitize=address -I.

# Folder to store executables
EXECFOLDER = execs
# Shared header for all programs
SHARED_H = cnf.h

# Programs
PROGRAMS = dpll gsat walksat

# Create the executables directory
$(EXECFOLDER):
	mkdir -p $(EXECFOLDER)

# Build all programs
all: $(EXECFOLDER) $(PROGRAMS)

# Rule to build each program
dpll: DPLL/dpll.cpp DPLL/dpll.h $(SHARED_H)
	$(CXX) $(CXXFLAGS) -o $(EXECFOLDER)/DPLL DPLL/dpll.cpp

gsat: GSAT/gsat.cpp GSAT/gsat.h $(SHARED_H)
	$(CXX) $(CXXFLAGS) -o $(EXECFOLDER)/GSAT GSAT/gsat.cpp

walksat: WALKSAT/walksat.cpp WALKSAT/walksat.h $(SHARED_H)
	$(CXX) $(CXXFLAGS) -o $(EXECFOLDER)/WalkSAT WalKSAT/walksat.cpp

# Clean rule to remove executables
clean:
	rm -rf $(EXECFOLDER)

.PHONY: all clean $(PROGRAMS)