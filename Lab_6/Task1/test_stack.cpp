#include <iostream>
#include <string>
#include "Stack.h"
using namespace std;

int main() {
    // char stack with a tiny size, so we can see it grow
    Stack<char> s1(2);
    s1.push('+');
    s1.push('*');
    s1.push('(');

    cout << "Top: " << s1.peek() << endl;
    cout << "Size: " << s1.size() << endl;
    cout << "Popped: " << s1.pop() << endl;
    cout << "Popped: " << s1.pop() << endl;
    cout << "Popped: " << s1.pop() << endl;
    cout << "Empty? " << s1.isEmpty() << endl;

    // double stack
    Stack<double> s2;
    s2.push(3.5);
    s2.push(4.0);
    cout << "Popped: " << s2.pop() << endl;

    // string stack
    Stack<string> s3;
    s3.push("a");
    s3.push("b");
    cout << "Popped: " << s3.pop() << endl;

    // popping an empty stack should give an error
    try {
        s1.pop();
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    return 0;
}
