#include <bits/stdc++.h>
using namespace std;

// void seidel(int a1, int a2, int a3, int b1, int b2, int b3, int c1, int c2, int c3, int d1, int d2, int d3){
void seidel(vector<vector<double>>& A, vector<double>& xyz, vector<double>& d){
    int n = A.size();

    // check for Strictly DD
    bool isDiagonalDominant = true;
    for (int i = 0; i < n; i++) {
        double rowSum = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) rowSum += abs(A[i][j]);
        }
        if (abs(A[i][i]) <= rowSum) {
            isDiagonalDominant = false;
            break;
        }
    }

    if (!isDiagonalDominant) {
        cout << "Warning: The matrix is not strictly diagonally dominant. The Gauss-Seidel method may not converge." << endl;
    }

    /*
    // old 3 degree system
    double x = 0; double y = 0; double z = 0;

    for(int i = 0; i<10; i++){
        x = (d1 - b1 * y - c1 * z) / (double)a1;
        y = (d2 - a2 * x - c2 * z) / (double)b2;
        z = (d3 - a3 * x - b3 * y) / (double)c3;

        cout << "Gauss-Seidel Iteration " << i + 1 << ": ";
        cout << "x = " << x << ", ";
        cout << "y = " << y << ", ";
        cout << "z = " << z << endl;
    }
    */

    // dynamic n-degree system
    vector<double> x_curr = xyz;

    for(int iter = 0; iter < 10; iter++){
        for(int i = 0; i < n; i++){
            if (abs(A[i][i]) < 1e-9) {
                cout << "Error: Zero or near-zero diagonal element encountered at index " << i << ". Gauss-Seidel method cannot proceed." << endl;
                return;
            }

            double sum = 0;
            for(int j = 0; j < n; j++){
                if(i != j) {
                    sum += A[i][j] * x_curr[j];
                }
            }
            x_curr[i] = (d[i] - sum) / A[i][i];
        }

        cout << "Gauss-Seidel Iteration " << iter + 1 << ": ";
        for(int i = 0; i < n; i++){
            cout << "x" << i+1 << " = " << x_curr[i] << (i == n-1 ? "" : ", ");
        }
        cout << endl;
    }
    xyz = x_curr;
}

int main(){

    /*
        hard coded input for:

        a1x1 + b1y1 + c1z1 = d1 -> diagonal dominant for x1
        a2x2 + b2y2 + c2z2 = d2 -> diagonal dominant for y2
        a3x3 + b3y3 + c3z3 = d3 -> diagonal dominant for z3
    */

    /*
    int a1 = 10;
    int a2 = 1;
    int a3 = 2;

    int b1 = 2;
    int b2 = 8;
    int b3 = 1;

    int c1 = 1;
    int c2 = 2;
    int c3 = 9;

    int d1 = 7;
    int d2 = -4;
    int d3 = 10;

    jacobi(a1,a2,a3,b1,b2,b3,c1,c2,c3,d1,d2,d3);
    */

    //using c++ vectors for matrix A and d also for x y z values
    int n;
    cout << "Enter the size of the system (n): ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> d(n);
    vector<double> xyz(n, 0.0); // Initial guesses set to 0

    cout << "Enter the elements of matrix A (row by row):" << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> A[i][j];
        }
    }

    cout << "Enter the elements of vector d (b vector):" << endl;
    for(int i = 0; i < n; i++){
        cin >> d[i];
    }

    seidel(A, xyz, d);

    return 0;
}
