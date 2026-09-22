#include "pattern_match.h"

int findPattern(const std::string& text, const std::string& pattern) {
    int n = text.size();
    int m = pattern.size();

    if (m == 0) return 0;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) {
            return i;
        }
    }
    return -1;
}