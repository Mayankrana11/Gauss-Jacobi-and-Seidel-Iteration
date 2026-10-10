# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Targets
TARGETS = jacobi seidel

# Default target
all: $(TARGETS)

# Build Jacobi
jacobi: src/jacobi.cpp
	$(CXX) $(CXXFLAGS) src/jacobi.cpp -o jacobi

# Build Seidel
seidel: src/seidel.cpp
	$(CXX) $(CXXFLAGS) src/seidel.cpp -o seidel

# Clean up build 
clean:
	del /f /q jacobi.exe seidel.exe 2>nul || true

.PHONY: all clean
