#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <string>
#include "Stack.h"
using namespace std;

// - Part A: bracket validator

// check if the closing bracket matches the opening bracket
bool matches(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '{' && close == '}') ||
           (open == '[' && close == ']');
}

// returns true if all brackets are balanced and correctly nested
bool isValidBrackets(string s) {
    Stack<char> st;

    for (int i = 0; i < s.length(); i++) {
        char c = s[i];

        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else if (c == ')' || c == '}' || c == ']') {
            // a closing bracket with nothing open is wrong
            if (st.isEmpty()) {
                return false;
            }
            char top = st.pop();
            if (!matches(top, c)) {
                return false;
            }
        }
    }

    // if something is still open, it was never closed
    return st.isEmpty();
}

//  Part B: HTML/XML tag validator

// stores the result: valid or not, and the list of errors
struct MarkupResult {
    bool valid;
    string errors[50];
    int errorCount;
};

// adds an error message to the result
void addError(MarkupResult& r, string message) {
    r.valid = false;
    if (r.errorCount < 50) {
        r.errors[r.errorCount] = message;
        r.errorCount++;
    }
}

// gets the tag name (stops at a space or /)
string getTagName(string content, int start) {
    string name = "";
    for (int k = start; k < content.length(); k++) {
        char c = content[k];
        if (c == ' ' || c == '/' || c == '\t' || c == '\n') {
            break;
        }
        name += c;
    }
    return name;
}

// checks that the tags are properly opened, closed and nested
MarkupResult validate_markup(string s) {
    MarkupResult r;
    r.valid = true;
    r.errorCount = 0;
    Stack<string> st;

    int i = 0;
    while (i < s.length()) {
        // skip normal text
        if (s[i] != '<') {
            i++;
            continue;
        }

        // find the end of the tag
        int j = s.find('>', i);
        if (j == string::npos) {
            addError(r, "Unclosed '<' at position " + to_string(i));
            break;
        }

        string content = s.substr(i + 1, j - i - 1);
        int tagPos = i;
        i = j + 1;

        if (content.empty()) {
            addError(r, "Empty tag at position " + to_string(tagPos));
            continue;
        }

        // skip comments and declarations like <!-- --> or <?xml ?>
        if (content[0] == '!' || content[0] == '?') {
            continue;
        }

        if (content[0] == '/') {
            // closing tag
            string name = getTagName(content, 1);
            if (name == "") {
                addError(r, "Empty tag at position " + to_string(tagPos));
                continue;
            }

            if (st.isEmpty()) {
                addError(r, "Unexpected closing tag </" + name + "> at position " + to_string(tagPos));
            } else if (st.peek() == name) {
                st.pop();
            } else {
                // look for the matching opening tag deeper in the stack
                Stack<string> temp;
                bool found = false;
                while (!st.isEmpty()) {
                    string t = st.pop();
                    if (t == name) {
                        found = true;
                        break;
                    }
                    temp.push(t);
                }

                if (found) {
                    // the tags above it were never closed
                    while (!temp.isEmpty()) {
                        addError(r, "Tag <" + temp.pop() + "> was not closed before </" + name + "> (position " + to_string(tagPos) + ")");
                    }
                } else {
                    // no matching opening tag, put everything back
                    while (!temp.isEmpty()) {
                        st.push(temp.pop());
                    }
                    addError(r, "Unexpected closing tag </" + name + "> at position " + to_string(tagPos));
                }
            }
        } else {
            // opening tag or self-closing tag
            string name = getTagName(content, 0);
            if (content[content.length() - 1] == '/') {
                continue;   // self-closing like <br/>, nothing to track
            }
            st.push(name);
        }
    }

    // anything left on the stack was never closed
    while (!st.isEmpty()) {
        addError(r, "Tag <" + st.pop() + "> was never closed");
    }

    return r;
}

#endif
