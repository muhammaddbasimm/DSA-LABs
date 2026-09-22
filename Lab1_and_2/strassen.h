#ifndef STRASSEN_H
#define STRASSEN_H

#include <vector>

using Matrix = std::vector<std::vector<long long>>;


Matrix strassenMultiply(const Matrix& A, const Matrix& B);

// Multiplies two square matrices using the standard (naive) O(n^3) algorithm.
Matrix standardMultiply(const Matrix& A, const Matrix& B);

#endif