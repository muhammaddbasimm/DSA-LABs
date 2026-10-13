#include <iostream>
#include <string>
#include "Expression.h"
using namespace std;

// prints an infix expression and its postfix form
void showPostfix(string expr) {
    cout << "Infix:   " << expr << endl;
    string result = infix_to_postfix(expr);
    cout << "Postfix: " << result << endl;
    cout << endl;
}

// prints an infix expression and its prefix form
void showPrefix(string expr) {
    cout << "Infix:  " << expr << endl;
    string result = infix_to_prefix(expr);
    cout << "Prefix: " << result << endl;
    cout << endl;
}

int main() {
    cout << "--- infix to postfix ---" << endl;
    showPostfix("A+B*C");
    showPostfix("(A+B)*C");
    showPostfix("A+B*C-D/E");
    showPostfix("12+3*(4-1)");
    showPostfix("10 / 2 - 3");
    showPostfix("2^3^2");

    try {
        showPostfix("(A+B");
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    cout << endl;
    cout << "--- infix to prefix ---" << endl;
    showPrefix("A+B*C");
    showPrefix("(A+B)*C");
    showPrefix("A+B*C-D/E");
    showPrefix("12+3*(4-1)");
    showPrefix("10 / 2 - 3");
    showPrefix("A-B-C");
    showPrefix("2^3^2");

    try {
        showPrefix("(A+B");
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    return 0;
}
