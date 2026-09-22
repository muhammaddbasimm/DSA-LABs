#include <iostream>
#include <vector>
#include "find_indices.h"
using namespace std;

void printVector(const vector<int>& v) {
    cout << "{ ";
    for (int x : v) cout << x << " ";
    cout << "}" << endl;
}

int main() {
    // Test 1: multiple occurrences
    vector<int> a1 = {1, 2, 3, 2, 4, 2};
    cout << "Test 1 (multiple occurrences): ";
    printVector(findAllIndices(a1, 2));   // expect { 1 3 5 }

    // Test 2: key not present
    vector<int> a2 = {1, 2, 3};
    cout << "Test 2 (key not present): ";
    printVector(findAllIndices(a2, 9));   // expect { } (empty)

    // Test 3: empty array
    vector<int> a3 = {};
    cout << "Test 3 (empty array): ";
    printVector(findAllIndices(a3, 5));   // expect { } (empty)

    return 0;
}