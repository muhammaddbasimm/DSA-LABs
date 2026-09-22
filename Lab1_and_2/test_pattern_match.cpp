#include <iostream>
#include "pattern_match.h"
using namespace std;

void printResult(const string& label, int result) {
    cout << label << ": " << result << endl;
}

int main() {
    // Test 1
    printResult("Test 1 (pattern at beginning)", findPattern("hello world", "hello"));

    // Test 2
    printResult("Test 2 (pattern at end)", findPattern("hello world", "world"));

    // Test 3
    printResult("Test 3 (pattern not present)", findPattern("hello world", "xyz"));

    // Test 4
    printResult("Test 4 (empty pattern)", findPattern("hello world", ""));

    return 0;
}