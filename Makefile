# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Targets
TARGETS = jacobi.exe seidel.exe test.exe

# Default target: compile everything
all: $(TARGETS)

# Build Jacobi
jacobi.exe: src/jacobi.cpp
	$(CXX) $(CXXFLAGS) src/jacobi.cpp -o jacobi.exe

# Build Seidel
seidel.exe: src/seidel.cpp
	$(CXX) $(CXXFLAGS) src/seidel.cpp -o seidel.exe

# Build Test
test.exe: src/test.cpp
	$(CXX) $(CXXFLAGS) src/test.cpp -o test.exe

# Run comparison
test: all
	./test.exe

# Clean up build files (Windows)
clean:
	del /f /q jacobi.exe seidel.exe test.exe test_input.txt jacobi_output.txt seidel_output.txt 2>nul || true

.PHONY: all test clean