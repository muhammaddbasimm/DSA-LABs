#include <iostream>
#include "pascals_triangle.h"
using namespace std;

void printTriangle(const vector<vector<long long>>& triangle) {
    for (const auto& row : triangle) {
        for (long long val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    
    cout << "Test 1 (n = 0):" << endl;
    auto t0 = generatePascalsTriangle(0);
    printTriangle(t0);
    cout << (t0.empty() ? "PASS (empty)" : "FAIL") << endl << endl;

    
    cout << "Test 2 (n = 1):" << endl;
    auto t1 = generatePascalsTriangle(1);
    printTriangle(t1);
    cout << endl;

    
    cout << "Test 3 (n = 5):" << endl;
    auto t5 = generatePascalsTriangle(5);
    printTriangle(t5);

    vector<long long> expectedRow5 = {1, 4, 6, 4, 1};
    cout << "Row 5 check: " << (t5[4] == expectedRow5 ? "PASS" : "FAIL") << endl;

    return 0;
}