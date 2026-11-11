
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    int n, choice;

    cout << "enter matrix size ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> L(n, vector<double>(n, 0));
    vector<vector<double>> U(n, vector<double>(n, 0));
    vector<double> b(n), x(n), y(n);

    cout << "enter matrix elements\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i][j];

    cout << "enter vector b\n";
    for (int i = 0; i < n; i++)
        cin >> b[i];

    cout << "\nchoose method\n";
    cout << "1 gaussian elimination\n";
    cout << "2 lu decomposition\n";
    cout << "3 cholesky decomposition\n";
    cout << "enter choice ";
    cin >> choice;

    // gaussian elimination

    if (choice == 1) {

        for (int k = 0; k < n; k++) {

            int p = k;

            for (int i = k + 1; i < n; i++)
                if (fabs(A[i][k]) > fabs(A[p][k]))
                    p = i;

            if (fabs(A[p][k]) < 1e-12) {
                cout << "matrix is singular";
                return 0;
            }

            swap(A[k], A[p]);
            swap(b[k], b[p]);

            for (int i = k + 1; i < n; i++) {
                double f = A[i][k] / A[k][k];

                for (int j = k; j < n; j++)
                    A[i][j] -= f * A[k][j];

                b[i] -= f * b[k];
            }
        }

        // print upper triangular matrix

        cout << "\nU matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << A[i][j] << " ";
            cout << endl;
        }

        // back substitution

        for (int i = n - 1; i >= 0; i--) {
            x[i] = b[i];

            for (int j = i + 1; j < n; j++)
                x[i] -= A[i][j] * x[j];

            x[i] /= A[i][i];
        }
    }

    // lu decomposition

    else if (choice == 2) {

        for (int i = 0; i < n; i++)
            L[i][i] = 1;

        for (int k = 0; k < n; k++) {

            int p = k;

            for (int i = k + 1; i < n; i++)
                if (fabs(A[i][k]) > fabs(A[p][k]))
                    p = i;

            if (fabs(A[p][k]) < 1e-12) {
                cout << "matrix is singular";
                return 0;
            }

            swap(A[k], A[p]);
            swap(b[k], b[p]);

            for (int j = 0; j < k; j++)
                swap(L[k][j], L[p][j]);

            for (int i = k + 1; i < n; i++) {
                L[i][k] = A[i][k] / A[k][k];

                for (int j = k; j < n; j++)
                    A[i][j] -= L[i][k] * A[k][j];
            }
        }

        // copy upper triangular matrix

        for (int i = 0; i < n; i++)
            for (int j = i; j < n; j++)
                U[i][j] = A[i][j];

        // forward substitution

        for (int i = 0; i < n; i++) {
            y[i] = b[i];

            for (int j = 0; j < i; j++)
                y[i] -= L[i][j] * y[j];

            y[i] /= L[i][i];
        }

        // back substitution

        for (int i = n - 1; i >= 0; i--) {
            x[i] = y[i];

            for (int j = i + 1; j < n; j++)
                x[i] -= U[i][j] * x[j];

            x[i] /= U[i][i];
        }

        cout << "\nL matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << L[i][j] << " ";
            cout << endl;
        }

        cout << "\nU matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << U[i][j] << " ";
            cout << endl;
        }
    }

    // cholesky decomposition

    else if (choice == 3) {

        // check symmetry

        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                if (fabs(A[i][j] - A[j][i]) > 1e-9) {
                    cout << "matrix is not symmetric";
                    return 0;
                }

        // calculate lower triangular matrix

        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= i; j++) {

                double sum = 0;

                for (int k = 0; k < j; k++)
                    sum += L[i][k] * L[j][k];

                if (i == j) {
                    if (A[i][i] - sum <= 1e-12) {
                        cout << "matrix is not positive definite";
                        return 0;
                    }

                    L[i][j] = sqrt(A[i][i] - sum);
                }
                else {
                    L[i][j] = (A[i][j] - sum) / L[j][j];
                }
            }
        }

        // forward substitution

        for (int i = 0; i < n; i++) {
            y[i] = b[i];

            for (int j = 0; j < i; j++)
                y[i] -= L[i][j] * y[j];

            y[i] /= L[i][i];
        }

        // back substitution using transpose of L

        for (int i = n - 1; i >= 0; i--) {
            x[i] = y[i];

            for (int j = i + 1; j < n; j++)
                x[i] -= L[j][i] * x[j];

            x[i] /= L[i][i];
        }

        cout << "\nL matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << L[i][j] << " ";
            cout << endl;
        }

        cout << "\nL transpose matrix\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << L[j][i] << " ";
            cout << endl;
        }
    }

    else {
        cout << "invalid choice";
        return 0;
    }

    // print final solution

    cout << "\nsolution x\n";
    for (int i = 0; i < n; i++)
        cout << x[i] << " ";

    cout << endl;

    return 0;
}
