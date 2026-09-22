#include "mode.h"
#include <unordered_map>

std::vector<int> findMode(const std::vector<int>& arr) {
    std::vector<int> result;

    if (arr.empty()) {
        return result; 
    }

    std::unordered_map<int, int> freq;
    for (int val : arr) {
        freq[val]++;
    }

    int maxFreq = 0;
    for (const auto& pair : freq) {
        if (pair.second > maxFreq) {
            maxFreq = pair.second;
        }
    }

    for (const auto& pair : freq) {
        if (pair.second == maxFreq) {
            result.push_back(pair.first);
        }
    }

    return result;
}