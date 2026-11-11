
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    int n, choice;

    cout << "enter matrix size ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> Q(n, vector<double>(n, 0));
    vector<vector<double>> R(n, vector<double>(n, 0));

    cout << "enter matrix elements\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i][j];

    cout << "\nchoose qr factorization method\n";
    cout << "1 gram schmidt\n";
    cout << "2 givens rotation\n";
    cout << "3 householder reflection\n";
    cout << "enter choice ";
    cin >> choice;

    // gram schmidt orthogonalization

    if (choice == 1) {

        for (int j = 0; j < n; j++) {

            for (int i = 0; i < n; i++)
                Q[i][j] = A[i][j];

            for (int k = 0; k < j; k++) {

                for (int i = 0; i < n; i++)
                    R[k][j] += Q[i][k] * A[i][j];

                for (int i = 0; i < n; i++)
                    Q[i][j] -= R[k][j] * Q[i][k];
            }

            double norm = 0;

            for (int i = 0; i < n; i++)
                norm += Q[i][j] * Q[i][j];

            R[j][j] = sqrt(norm);

            if (R[j][j] < 1e-12) {
                cout << "matrix is rank deficient";
                return 0;
            }

            for (int i = 0; i < n; i++)
                Q[i][j] /= R[j][j];
        }
    }

    // givens rotation

    else if (choice == 2) {

        for (int i = 0; i < n; i++)
            Q[i][i] = 1;

        for (int j = 0; j < n; j++) {

            for (int i = n - 1; i > j; i--) {

                double a = A[j][j];
                double b = A[i][j];

                double r = sqrt(a * a + b * b);

                if (r < 1e-12) continue;

                double c = a / r;
                double s = b / r;

                for (int k = 0; k < n; k++) {
                    double x = A[j][k];
                    double y = A[i][k];

                    A[j][k] = c * x + s * y;
                    A[i][k] = -s * x + c * y;
                }

                for (int k = 0; k < n; k++) {
                    double x = Q[k][j];
                    double y = Q[k][i];

                    Q[k][j] = c * x + s * y;
                    Q[k][i] = -s * x + c * y;
                }
            }
        }

        // copy R

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                R[i][j] = A[i][j];
    }

    // householder reflection

    else if (choice == 3) {

        for (int i = 0; i < n; i++)
            Q[i][i] = 1;

        for (int k = 0; k < n - 1; k++) {

            vector<double> v(n, 0);
            double norm = 0;

            for (int i = k; i < n; i++)
                norm += A[i][k] * A[i][k];

            norm = sqrt(norm);

            if (norm < 1e-12) continue;

            for (int i = k; i < n; i++)
                v[i] = A[i][k];

            v[k] += (v[k] >= 0 ? norm : -norm);

            double vnorm = 0;

            for (int i = k; i < n; i++)
                vnorm += v[i] * v[i];

            for (int j = k; j < n; j++) {

                double dot = 0;

                for (int i = k; i < n; i++)
                    dot += v[i] * A[i][j];

                for (int i = k; i < n; i++)
                    A[i][j] -= 2 * v[i] * dot / vnorm;
            }

            for (int i = 0; i < n; i++) {

                double dot = 0;

                for (int j = k; j < n; j++)
                    dot += Q[i][j] * v[j];

                for (int j = k; j < n; j++)
                    Q[i][j] -= 2 * dot * v[j] / vnorm;
            }
        }

        // copy R

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                R[i][j] = A[i][j];
    }

    else {
        cout << "invalid choice";
        return 0;
    }

    // print Q matrix

    cout << "\nQ matrix\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << Q[i][j] << " ";
        cout << endl;
    }

    // print R matrix

    cout << "\nR matrix\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << R[i][j] << " ";
        cout << endl;
    }

    return 0;
}
