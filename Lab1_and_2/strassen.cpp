#include "strassen.h"



static Matrix addMatrix(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, std::vector<long long>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

static Matrix subtractMatrix(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, std::vector<long long>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}


static void splitMatrix(const Matrix& M, Matrix& A11, Matrix& A12, Matrix& A21, Matrix& A22) {
    int n = M.size();
    int half = n / 2;
    A11 = Matrix(half, std::vector<long long>(half));
    A12 = Matrix(half, std::vector<long long>(half));
    A21 = Matrix(half, std::vector<long long>(half));
    A22 = Matrix(half, std::vector<long long>(half));

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            A11[i][j] = M[i][j];
            A12[i][j] = M[i][j + half];
            A21[i][j] = M[i + half][j];
            A22[i][j] = M[i + half][j + half];
        }
    }
}


static Matrix combineMatrix(const Matrix& C11, const Matrix& C12, const Matrix& C21, const Matrix& C22) {
    int half = C11.size();
    int n = half * 2;
    Matrix C(n, std::vector<long long>(n));

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[i][j] = C11[i][j];
            C[i][j + half] = C12[i][j];
            C[i + half][j] = C21[i][j];
            C[i + half][j + half] = C22[i][j];
        }
    }
    return C;
}



Matrix standardMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}



Matrix strassenMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();

    // Base case: 1x1 matrix
    if (n == 1) {
        Matrix C(1, std::vector<long long>(1));
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    Matrix A11, A12, A21, A22, B11, B12, B21, B22;
    splitMatrix(A, A11, A12, A21, A22);
    splitMatrix(B, B11, B12, B21, B22);

    Matrix M1 = strassenMultiply(addMatrix(A11, A22), addMatrix(B11, B22));
    Matrix M2 = strassenMultiply(addMatrix(A21, A22), B11);
    Matrix M3 = strassenMultiply(A11, subtractMatrix(B12, B22));
    Matrix M4 = strassenMultiply(A22, subtractMatrix(B21, B11));
    Matrix M5 = strassenMultiply(addMatrix(A11, A12), B22);
    Matrix M6 = strassenMultiply(subtractMatrix(A21, A11), addMatrix(B11, B12));
    Matrix M7 = strassenMultiply(subtractMatrix(A12, A22), addMatrix(B21, B22));

    Matrix C11 = addMatrix(subtractMatrix(addMatrix(M1, M4), M5), M7);
    Matrix C12 = addMatrix(M3, M5);
    Matrix C21 = addMatrix(M2, M4);
    Matrix C22 = addMatrix(subtractMatrix(addMatrix(M1, M3), M2), M6);

    return combineMatrix(C11, C12, C21, C22);
}