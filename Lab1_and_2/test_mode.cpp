#include <iostream>
#include <vector>
#include <algorithm>
#include "mode.h"
using namespace std;

void printModes(const vector<int>& modes) {
    cout << "{ ";
    for (int m : modes) cout << m << " ";
    cout << "}" << endl;
}

int main() {
    
    vector<int> a1 = {1, 2, 2, 3, 2, 4};
    cout << "Test 1 (unique mode): ";
    printModes(findMode(a1)); 

    
    vector<int> a2 = {1, 1, 2, 2, 3};
    cout << "Test 2 (multiple modes): ";
    auto modes2 = findMode(a2);
    sort(modes2.begin(), modes2.end());
    printModes(modes2); 

   
    vector<int> a3 = {};
    cout << "Test 3 (empty array): ";
    printModes(findMode(a3)); 

    return 0;
}