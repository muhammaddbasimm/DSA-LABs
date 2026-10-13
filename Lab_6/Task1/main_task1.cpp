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

// prints a postfix expression and its infix form
void showPostfixToInfix(string expr) {
    cout << "Postfix: " << expr << endl;
    string result = postfix_to_infix(expr);
    cout << "Infix:   " << result << endl;
    cout << endl;
}

// prints a postfix expression and its answer
void showEvalPostfix(string expr) {
    cout << "Postfix: " << expr << endl;
    double result = evaluate_postfix(expr);
    cout << "Answer:  " << result << endl;
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

    cout << endl;
    cout << "--- postfix to infix ---" << endl;
    showPostfixToInfix("A B C * +");
    showPostfixToInfix("A B + C *");
    showPostfixToInfix("12 3 4 1 - * +");
    showPostfixToInfix("2 3 2 ^ ^");

    try {
        showPostfixToInfix("A +");
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    cout << endl;
    cout << "--- evaluate postfix ---" << endl;
    showEvalPostfix("2 3 4 * +");
    showEvalPostfix("12 3 4 1 - * +");
    showEvalPostfix("10 2 / 3 -");
    showEvalPostfix("2 3 2 ^ ^");
    showEvalPostfix("5.5 2 *");

    try {
        showEvalPostfix("4 0 /");
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    return 0;
}
