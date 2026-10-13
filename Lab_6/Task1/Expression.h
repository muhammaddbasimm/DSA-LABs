#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <stdexcept>
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
bool shouldPop(char top, char current) {
    if (precedence(top) > precedence(current)) return true;
    // for equal priority, pop only if the operator is not ^
    // (^ is calculated right to left)
    if (precedence(top) == precedence(current) && current != '^') return true;
    return false;
}

// converts an infix expression to postfix
string infix_to_postfix(string expr) {
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
            while (!st.isEmpty() && isOperator(st.peek()) && shouldPop(st.peek(), c)) {
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

#endif
