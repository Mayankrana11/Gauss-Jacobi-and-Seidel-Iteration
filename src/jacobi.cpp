#include <bits/stdc++.h>
using namespace std;

// void jacobi(int a1, int a2, int a3, int b1, int b2, int b3, int c1, int c2, int c3, int d1, int d2, int d3){
void jacobi(vector<vector<double>>& A, vector<double>& xyz, vector<double>& d){
    //main declare rn would be dynamic/global later

    //init
    /*
    double x = 0; double y = 0; double z = 0;

    //iter
    for(int i = 0; i<10; i++){
        double newx = (d1 - b1 * y - c1 * z) / (double)a1;
        double newy = (d2 - a2 * x - c2 * z) / (double)b2;
        double newz = (d3 - a3 * x - b3 * y) / (double)c3;

        x = newx;
        y = newy;
        z = newz;

        cout << "Gauss-Jacobi Iteration " << i + 1 << ": ";
        cout << "x = " << x << ", ";
        cout << "y = " << y << ", ";
        cout << "z = " << z << endl;
    }
    */

    int n = A.size();
    vector<double> x_old = xyz;
    vector<double> x_new(n);

    for(int iter = 0; iter < 10; iter++){
        for(int i = 0; i < n; i++){
            if (abs(A[i][i]) < 1e-9) {
                cout << "Error: Zero or near-zero diagonal element encountered at index " << i << ". Jacobi method cannot proceed." << endl;
                return;
            }
            double sum = 0;
            for(int j = 0; j < n; j++){
                if(i != j) {
                    sum += A[i][j] * x_old[j];
                }
            }
            x_new[i] = (d[i] - sum) / A[i][i];
        }

        x_old = x_new;

        cout << "Gauss-Jacobi Iteration " << iter + 1 << ": ";
        for(int i = 0; i < n; i++){
            cout << "x" << i+1 << " = " << x_old[i] << (i == n-1 ? "" : ", ");
        }
        cout << endl;
    }
    xyz = x_old;
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

    jacobi(A, xyz, d);

    return 0;
}
