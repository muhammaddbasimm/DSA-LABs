#include <iostream>
#include <cstdlib>
#include <ctime>
#include "strassen.h"
using namespace std;

void printMatrix(const Matrix& M) {
    for (const auto& row : M) {
        for (long long val : row) cout << val << " ";
        cout << endl;
    }
}

bool matricesEqual(const Matrix& A, const Matrix& B) {
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); i++)
        for (size_t j = 0; j < A.size(); j++)
            if (A[i][j] != B[i][j]) return false;
    return true;
}

Matrix randomMatrix(int n) {
    Matrix M(n, vector<long long>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            M[i][j] = rand() % 10; // small values 0-9
    return M;
}

int main() {
    srand((unsigned)time(nullptr));

   
    Matrix A2 = {{1, 2}, {3, 4}};
    Matrix B2 = {{5, 6}, {7, 8}};
    Matrix strassenResult2 = strassenMultiply(A2, B2);
    Matrix standardResult2 = standardMultiply(A2, B2);
    cout << "Test 1 (2x2 multiplication): "
         << (matricesEqual(strassenResult2, standardResult2) ? "PASS" : "FAIL") << endl;
    printMatrix(strassenResult2);
    cout << endl;

  
    Matrix A4 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    Matrix B4 = {
        {16, 15, 14, 13},
        {12, 11, 10, 9},
        {8, 7, 6, 5},
        {4, 3, 2, 1}
    };
    Matrix strassenResult4 = strassenMultiply(A4, B4);
    Matrix standardResult4 = standardMultiply(A4, B4);
    cout << "Test 2 (4x4 multiplication): "
         << (matricesEqual(strassenResult4, standardResult4) ? "PASS" : "FAIL") << endl;
    printMatrix(strassenResult4);
    cout << endl;

    
    int n = 4; // must be power of 2
    Matrix randA = randomMatrix(n);
    Matrix randB = randomMatrix(n);
    Matrix strassenRand = strassenMultiply(randA, randB);
    Matrix standardRand = standardMultiply(randA, randB);
    cout << "Test 3 (random " << n << "x" << n << " vs naive): "
         << (matricesEqual(strassenRand, standardRand) ? "PASS" : "FAIL") << endl;

    return 0;
}