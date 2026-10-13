#include <iostream>
#include <string>
#include "Validators.h"
using namespace std;

// tests the bracket validator
void showBrackets(string s) {
    cout << "Input:  " << s << endl;
    if (isValidBrackets(s)) {
        cout << "Result: Valid" << endl;
    } else {
        cout << "Result: Not valid" << endl;
    }
    cout << endl;
}

// tests the markup validator
void showMarkup(string s) {
    MarkupResult r = validate_markup(s);
    cout << "Markup: " << s << endl;
    if (r.valid) {
        cout << "Result: Valid" << endl;
    } else {
        cout << "Result: Not valid" << endl;
        for (int i = 0; i < r.errorCount; i++) {
            cout << "  - " << r.errors[i] << endl;
        }
    }
    cout << endl;
}

int main() {
    cout << "Part A: Bracket Validator" << endl;
    showBrackets("()");
    showBrackets("{[()]}");
    showBrackets("a(b[c]{d})e");
    showBrackets("([)]");
    showBrackets("((");
    showBrackets("())");
    showBrackets(")(");

    cout << "Part B: Markup Validator" << endl;
    showMarkup("<html><body><p>Hello</p></body></html>");
    showMarkup("<div><br/><img src=\"a.png\"/></div>");
    showMarkup("<div><span>Hi</div>");
    showMarkup("<div><p>text</div></p>");
    showMarkup("<ul><li>One</li>");
    showMarkup("</p>");
    showMarkup("<div");

    return 0;
}
