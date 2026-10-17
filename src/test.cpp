
#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

const double TOLERANCE = 1e-4;
const int N = 3;

const vector<double> exactSolution = {1.0, 2.0, -1.0};

struct Result {
    string method;
    int iterations = 0;
    double errorPercent = 0.0;
    double absoluteError = 0.0;
    double timeMs = 0.0;
    bool converged = false;
};

void createInputFile() {
    ofstream file("test_input.txt");

    file << N << '\n';
    file << "10 -1 2\n";
    file << "-1 11 -1\n";
    file << "2 -1 10\n";
    file << "6 22 -10\n";

    file.close();
}

double calculateError(const vector<double>& x) {
    double error = 0.0;

    for (int i = 0; i < N; i++) {
        error += abs(x[i] - exactSolution[i]);
    }

    return error;
}

double calculatePercentageError(const vector<double>& x) {
    double error = calculateError(x);
    double denominator = 0.0;

    for (double value : exactSolution) {
        denominator += abs(value);
    }

    return (error / denominator) * 100.0;
}

double runExecutable(const string& exe,
                     const string& outputFile) {
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


Result analyseOutput(const string& method,
                     const string& outputFile,
                     double timeMs) {
    Result result;
    result.method = method;
    result.timeMs = timeMs;

    ifstream file(outputFile);

    if (!file.is_open()) {
        cerr << "Cannot open " << outputFile << '\n';
        exit(1);
    }

    string line;
    vector<double> x(N, 0.0);

    while (getline(file, line)) {
        // Find iteration number without regex.
        size_t pos = line.find("Iteration ");
        if (pos == string::npos) {
            continue;
        }

        pos += string("Iteration ").length();

        int iteration;
        try {
            iteration = stoi(line.substr(pos));
        } catch (...) {
            continue;
        }

        vector<double> currentX;
        size_t searchPos = 0;

        // Extract x1, x2, ... values.
        while (true) {
            size_t xPos = line.find("x", searchPos);

            if (xPos == string::npos) {
                break;
            }

            size_t equalPos = line.find("=", xPos);

            if (equalPos == string::npos) {
                break;
            }

            // Make sure this is a variable such as x1, not other text.
            if (xPos + 1 >= line.size() ||
                !isdigit(static_cast<unsigned char>(line[xPos + 1]))) {
                searchPos = xPos + 1;
                continue;
            }

            size_t valueStart = equalPos + 1;

            while (valueStart < line.size() &&
                   isspace(static_cast<unsigned char>(line[valueStart]))) {
                valueStart++;
            }

            size_t valueEnd = valueStart;

            while (valueEnd < line.size() &&
                   (isdigit(static_cast<unsigned char>(line[valueEnd])) ||
                    line[valueEnd] == '-' ||
                    line[valueEnd] == '+' ||
                    line[valueEnd] == '.' ||
                    line[valueEnd] == 'e' ||
                    line[valueEnd] == 'E')) {
                valueEnd++;
            }

            if (valueEnd > valueStart) {
                try {
                    currentX.push_back(
                        stod(line.substr(valueStart, valueEnd - valueStart))
                    );
                } catch (...) {
                    // Ignore malformed values.
                }
            }

            searchPos = valueEnd;
        }

        if (static_cast<int>(currentX.size()) != N) {
            cerr << "Failed to parse iteration " << iteration
                 << " from " << outputFile << '\n';
            exit(1);
        }

        x = currentX;

        double error = calculateError(x);

        if (!result.converged && error < TOLERANCE) {
            result.converged = true;
            result.iterations = iteration;
            result.absoluteError = error;
            result.errorPercent = calculatePercentageError(x);
        }
    }

    if (!result.converged) {
        result.iterations = 10;
        result.absoluteError = calculateError(x);
        result.errorPercent = calculatePercentageError(x);
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

int main() {
    createInputFile();

    double jacobiTime =
        runExecutable(".\\jacobi.exe", "jacobi_output.txt");

    double seidelTime =
        runExecutable(".\\seidel.exe", "seidel_output.txt");

    Result jacobi = analyseOutput(
        "Jacobi method", "jacobi_output.txt", jacobiTime
    );

    Result seidel = analyseOutput(
        "Gauss-Seidel method", "seidel_output.txt", seidelTime
    );

    cout << "\n========== JACOBI ITERATIONS ==========\n\n";
    printFullOutput("jacobi_output.txt");

    cout << "\n======= GAUSS-SEIDEL ITERATIONS =======\n\n";
    printFullOutput("seidel_output.txt");

    cout << "\n========== FINAL COMPARISON ==========\n\n";

    cout << left
         << setw(25) << "Methods"
         << setw(20) << "No of iteration"
         << setw(18) << "Error %"
         << setw(18) << "Time (ms)"
         << "Status\n";

    cout << string(85, '-') << '\n';

    auto printResult = [](const Result& r) {
        cout << left
             << setw(25) << r.method
             << setw(20) << r.iterations
             << fixed << setprecision(6)
             << setw(18) << r.errorPercent
             << setw(18) << r.timeMs
             << (r.converged ? "Tolerance reached" : "Not reached")
             << '\n';
    };

    printResult(jacobi);
    printResult(seidel);

    cout << "\nTolerance: Total absolute error < "
         << scientific << TOLERANCE << '\n';

    cout << "Percentage error = (Total absolute error / "
            "Sum of absolute exact values) * 100\n";

    cout << "\nNote: The iteration count is the first iteration "
            "meeting the tolerance, or the final iteration "
            "if tolerance was not reached.\n";

    return 0;
}
