#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

const double TOLERANCE = 1e-4;

struct TestCase {
    int n;
    vector<vector<double>> matrix;
    vector<double> b;
    vector<double> exactSolution;
};

const vector<TestCase> TEST_CASES = {
    {
        3,
        {{10, -1, 2}, {-1, 11, -1}, {2, -1, 10}},
        {6, 22, -10},
        {1.0, 2.0, -1.0}
    },
    {
        2,
        {{4, 1}, {1, 3}},
        {5, 4},
        {1.0, 1.0}
    },
    {
        4,
        {{10, 2, 3, 1}, {1, 15, 1, 2}, {2, 3, 20, 1}, {1, 2, 1, 10}},
        {16, 19, 24, 14},
        {1.0, 1.0, 1.0, 1.0}
    },
    {
        5,
        {{10, 1, 1, 1, 1}, {1, 10, 1, 1, 1}, {1, 1, 10, 1, 1}, {1, 1, 1, 10, 1}, {1, 1, 1, 1, 10}},
        {14, 14, 14, 14, 14},
        {1.0, 1.0, 1.0, 1.0, 1.0}
    }
};

struct Result {
    string method;
    int iterations = 0;
    double errorPercent = 0.0;
    double absoluteError = 0.0;
    double timeMs = 0.0;
    bool converged = false;
};

void createInputFile(const TestCase& tc) {
    ofstream file("test_input.txt");
    file << tc.n << '\n';
    for (const auto& row : tc.matrix) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i] << (i == row.size() - 1 ? "" : " ");
        }
        file << '\n';
    }
    for (size_t i = 0; i < tc.b.size(); ++i) {
        file << tc.b[i] << (i == tc.b.size() - 1 ? "" : " ");
    }
    file << '\n';
    file.close();
}

double calculateError(const vector<double>& x, const vector<double>& exact) {
    double error = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        error += abs(x[i] - exact[i]);
    }
    return error;
}

double calculatePercentageError(const vector<double>& x, const vector<double>& exact) {
    double error = calculateError(x, exact);
    double denominator = 0.0;
    for (double value : exact) {
        denominator += abs(value);
    }
    return (error / denominator) * 100.0;
}

double runExecutable(const string& exe, const string& outputFile) {
    string command = exe + " < test_input.txt > " + outputFile;
    auto start = high_resolution_clock::now();
    int status = system(command.c_str());
    auto stop = high_resolution_clock::now();
    if (status != 0) {
        cerr << "Failed to execute " << exe << '\n';
        exit(1);
    }
    return duration<double, milli>(stop - start).count();
}

Result analyseOutput(const string& method, const string& outputFile, double timeMs, const TestCase& tc) {
    Result result;
    result.method = method;
    result.timeMs = timeMs;
    ifstream file(outputFile);
    if (!file.is_open()) {
        cerr << "Cannot open " << outputFile << '\n';
        exit(1);
    }
    string line;
    vector<double> x(tc.n, 0.0);
    while (getline(file, line)) {
        size_t pos = line.find("Iteration ");
        if (pos == string::npos) continue;
        pos += string("Iteration ").length();
        int iteration;
        try {
            iteration = stoi(line.substr(pos));
        } catch (...) {
            continue;
        }
        vector<double> currentX;
        size_t searchPos = 0;
        while (true) {
            size_t xPos = line.find("x", searchPos);
            if (xPos == string::npos) break;
            size_t equalPos = line.find("=", xPos);
            if (equalPos == string::npos) break;
            if (xPos + 1 >= line.size() || !isdigit(static_cast<unsigned char>(line[xPos + 1]))) {
                searchPos = xPos + 1;
                continue;
            }
            size_t valueStart = equalPos + 1;
            while (valueStart < line.size() && isspace(static_cast<unsigned char>(line[valueStart]))) {
                valueStart++;
            }
            size_t valueEnd = valueStart;
            while (valueEnd < line.size() && (isdigit(static_cast<unsigned char>(line[valueEnd])) || line[valueEnd] == '-' || line[valueEnd] == '+' || line[valueEnd] == '.' || line[valueEnd] == 'e' || line[valueEnd] == 'E')) {
                valueEnd++;
            }
            if (valueEnd > valueStart) {
                try {
                    currentX.push_back(stod(line.substr(valueStart, valueEnd - valueStart)));
                } catch (...) {}
            }
            searchPos = valueEnd;
        }
        if (static_cast<int>(currentX.size()) != tc.n) {
            cerr << "Failed to parse iteration " << iteration << " from " << outputFile << '\n';
            exit(1);
        }
        x = currentX;
        double error = calculateError(x, tc.exactSolution);
        if (!result.converged && error < TOLERANCE) {
            result.converged = true;
            result.iterations = iteration;
            result.absoluteError = error;
            result.errorPercent = calculatePercentageError(x, tc.exactSolution);
        }
    }
    if (!result.converged) {
        result.iterations = 10;
        result.absoluteError = calculateError(x, tc.exactSolution);
        result.errorPercent = calculatePercentageError(x, tc.exactSolution);
    }
    return result;
}

void printFullOutput(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        cout << line << '\n';
    }
}

int main(int argc, char* argv[]) {
    int testIdx = 0;
    if (argc > 1) {
        try {
            testIdx = stoi(argv[1]);
        } catch (...) {
            cerr << "Invalid test case index. Using 0.\n";
        }
    }

    if (testIdx < 0 || testIdx >= static_cast<int>(TEST_CASES.size())) {
        cerr << "Test case index " << testIdx << " out of range. Available: 0 to " << TEST_CASES.size() - 1 << '\n';
        return 1;
    }

    const TestCase& tc = TEST_CASES[testIdx];
    cout << "Running Test Case " << testIdx << " (n=" << tc.n << ")\n";
    createInputFile(tc);

    double jacobiTime = runExecutable(".\\jacobi.exe", "jacobi_output.txt");
    double seidelTime = runExecutable(".\\seidel.exe", "seidel_output.txt");

    Result jacobi = analyseOutput("Jacobi method", "jacobi_output.txt", jacobiTime, tc);
    Result seidel = analyseOutput("Gauss-Seidel method", "seidel_output.txt", seidelTime, tc);

    cout << "\n========== JACOBI ITERATIONS ==========\n\n";
    printFullOutput("jacobi_output.txt");
    cout << "\n======= GAUSS-SEIDEL ITERATIONS =======\n\n";
    printFullOutput("seidel_output.txt");

    cout << "\n========== FINAL COMPARISON ==========\n\n";
    cout << left << setw(25) << "Methods" << setw(20) << "No of iteration" << setw(18) << "Error %" << setw(18) << "Time (ms)" << "Status\n";
    cout << string(85, '-') << '\n';
    auto printResult = [](const Result& r) {
        cout << left << setw(25) << r.method << setw(20) << r.iterations << fixed << setprecision(6) << setw(18) << r.errorPercent << setw(18) << r.timeMs << (r.converged ? "Tolerance reached" : "Not reached") << '\n';
    };
    printResult(jacobi);
    printResult(seidel);

    cout << "\nTolerance: Total absolute error < " << scientific << TOLERANCE << '\n';
    cout << "Percentage error = (Total absolute error / Sum of absolute exact values) * 100\n";
    return 0;
}
