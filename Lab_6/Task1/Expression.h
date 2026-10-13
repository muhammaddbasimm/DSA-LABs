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

#endif
