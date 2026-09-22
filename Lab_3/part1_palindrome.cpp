#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool isRelevant(char c) {
return isalnum((unsigned char)c);
}

bool isPalindrome(string input) {
int left = 0;
int right = input.length() - 1;
while (left < right) {
while (left < right && !isRelevant(input[left])) left++;
while (left < right && !isRelevant(input[right])) right--;
if (tolower(input[left]) != tolower(input[right])) return false;
left++;
right--;
}
return true;
}

int main() {
string input;
cout << "Enter a string: ";
getline(cin, input);
if (isPalindrome(input))
cout << input << " is a palindrome" << endl;
else
cout << input << " is not a palindrome" << endl;
return 0;
}
