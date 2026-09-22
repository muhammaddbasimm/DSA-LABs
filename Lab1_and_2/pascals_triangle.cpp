#include "pascals_triangle.h"

std::vector<std::vector<long long>> generatePascalsTriangle(int n) {
    std::vector<std::vector<long long>> triangle;

    for (int row = 0; row < n; row++) {
        std::vector<long long> currentRow(row + 1);
        currentRow[0] = currentRow[row] = 1;

        for (int col = 1; col < row; col++) {
            currentRow[col] = triangle[row - 1][col - 1] + triangle[row - 1][col];
        }

        triangle.push_back(currentRow);
    }

    return triangle;
}