#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <stdexcept>
#include <cmath>
#include "Stack.h"
using namespace std;

// check if a character is + - * / ^
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// check if a character is part of a number or a name (like 12 or A)
bool isOperand(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') || c == '.';
}

// higher number = higher priority
int precedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

// should the operator on the stack be moved to the output first?
// forPrefix is true when we are converting to prefix (reversed scan)
bool shouldPop(char top, char current, bool forPrefix) {
    if (precedence(top) > precedence(current)) return true;
    if (precedence(top) == precedence(current)) {
        if (forPrefix) return current == '^';
        return current != '^';
    }
    return false;
}

// does the main conversion work (used for both postfix and prefix)
string convertToPostfix(string expr, bool forPrefix) {
    Stack<char> st;
    string result = "";
    int i = 0;

    while (i < expr.length()) {
        char c = expr[i];

        // skip spaces
        if (c == ' ') {
            i++;
            continue;
        }

        // operand: copy the whole number or name to the output
        if (isOperand(c)) {
            while (i < expr.length() && isOperand(expr[i])) {
                result += expr[i];
                i++;
            }
            result += ' ';
            continue;
        }

        if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            // pop everything until we find the matching (
            while (!st.isEmpty() && st.peek() != '(') {
                result += st.pop();
                result += ' ';
            }
            if (st.isEmpty()) {
                throw runtime_error("Mismatched brackets");
            }
            st.pop();   // remove the (
        } else if (isOperator(c)) {
            // move higher priority operators to the output first
            while (!st.isEmpty() && isOperator(st.peek()) && shouldPop(st.peek(), c, forPrefix)) {
                result += st.pop();
                result += ' ';
            }
            st.push(c);
        } else {
            throw runtime_error("Invalid character in expression");
        }

        i++;
    }

    // move the remaining operators to the output
    while (!st.isEmpty()) {
        if (st.peek() == '(') {
            throw runtime_error("Mismatched brackets");
        }
        result += st.pop();
        result += ' ';
    }

    // remove the extra space at the end
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

// the postfix function the lab asks for (uses the normal rules)
string infix_to_postfix(string expr) {
    return convertToPostfix(expr, false);
}

// reverses the order of the tokens (a number like 12 stays 12)
// if swapBrackets is true, ( becomes ) and ) becomes (
string reverseTokens(string expr, bool swapBrackets) {
    Stack<string> st;
    int i = 0;

    while (i < expr.length()) {
        char c = expr[i];

        if (c == ' ') {
            i++;
            continue;
        }

        string token = "";
        if (isOperand(c)) {
            while (i < expr.length() && isOperand(expr[i])) {
                token += expr[i];
                i++;
            }
        } else {
            token = string(1, c);
            if (swapBrackets && c == '(') token = ")";
            else if (swapBrackets && c == ')') token = "(";
            i++;
        }
        st.push(token);
    }

    // popping the stack gives the tokens in reverse order
    string result = "";
    while (!st.isEmpty()) {
        result += st.pop();
        result += ' ';
    }
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

// converts an infix expression to prefix
string infix_to_prefix(string expr) {
    string reversed = reverseTokens(expr, true);
    string postfix = convertToPostfix(reversed, true);
    return reverseTokens(postfix, false);
}

// splits a string into tokens (separated by spaces)
// the tokens are stored in the array and the count is returned
int splitTokens(string expr, string tokens[], int maxTokens) {
    int count = 0;
    string current = "";

    for (int i = 0; i <= expr.length(); i++) {
        if (i == expr.length() || expr[i] == ' ') {
            if (current != "") {
                if (count == maxTokens) {
                    throw runtime_error("Expression is too long");
                }
                tokens[count] = current;
                count++;
                current = "";
            }
        } else {
            current += expr[i];
        }
    }
    return count;
}

// a token is an operator if it is a single operator character
bool isOperatorToken(string t) {
    return t.length() == 1 && isOperator(t[0]);
}

// converts a postfix expression (with spaces) to infix
string postfix_to_infix(string expr) {
    string tokens[200];
    int n = splitTokens(expr, tokens, 200);
    Stack<string> st;

    for (int i = 0; i < n; i++) {
        if (isOperatorToken(tokens[i])) {
            if (st.size() < 2) {
                throw runtime_error("Invalid postfix expression");
            }
            string b = st.pop();
            string a = st.pop();
            st.push("(" + a + tokens[i] + b + ")");
        } else {
            st.push(tokens[i]);
        }
    }

    if (st.size() != 1) {
        throw runtime_error("Invalid postfix expression");
    }
    return st.pop();
}

// does one calculation: a op b
double calculate(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) {
            throw runtime_error("Division by zero");
        }
        return a / b;
    }
    return pow(a, b);   // the only one left is ^
}

// evaluates a postfix expression (numbers only, with spaces)
double evaluate_postfix(string expr) {
    string tokens[200];
    int n = splitTokens(expr, tokens, 200);
    Stack<double> st;

    for (int i = 0; i < n; i++) {
        if (isOperatorToken(tokens[i])) {
            if (st.size() < 2) {
                throw runtime_error("Invalid postfix expression");
            }
            double b = st.pop();
            double a = st.pop();
            st.push(calculate(a, b, tokens[i][0]));
        } else {
            st.push(stod(tokens[i]));   // turn the text into a number
        }
    }

    if (st.size() != 1) {
        throw runtime_error("Invalid postfix expression");
    }
    return st.pop();
}

// evaluates a prefix expression (numbers only, with spaces)
double evaluate_prefix(string expr) {
    string tokens[200];
    int n = splitTokens(expr, tokens, 200);
    Stack<double> st;

    // go from the last token to the first
    for (int i = n - 1; i >= 0; i--) {
        if (isOperatorToken(tokens[i])) {
            if (st.size() < 2) {
                throw runtime_error("Invalid prefix expression");
            }
            double a = st.pop();
            double b = st.pop();
            st.push(calculate(a, b, tokens[i][0]));
        } else {
            st.push(stod(tokens[i]));
        }
    }

    if (st.size() != 1) {
        throw runtime_error("Invalid prefix expression");
    }
    return st.pop();
}

#endif
