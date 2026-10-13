#include <iostream>
#include <string>
#include "Expression.h"
using namespace std;

// prints an infix expression and its postfix form
void showPostfix(string expr) {
    cout << "Infix:   " << expr << endl;
    cout << "Postfix: " << infix_to_postfix(expr) << endl;
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

    // a wrong expression should give an error
    try {
        showPostfix("(A+B");
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    return 0;
}
