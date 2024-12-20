#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
#include <unordered_map>
#include <unordered_set>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};  // Initialize output array
    int totalMatchLength = 0;                      // Total length of exact matches
    int longestApproxMatch = 20;                    // Length of longest approximate match
    int longestApproxMatchStart1 = -1;             // Start index in submission1
    int longestApproxMatchStart2 = -1;             // Start index in submission2
    int n1 = submission1.size();
    int n2 = submission2.size();
    std::vector<bool> used1(n1, false);
    std::vector<bool> used2(n2, false);

    // Function to mark indices in a given range as used
    auto mark_used = [](std::vector<bool>& used, int start, int length) {
        for (int i = start; i < start + length; ++i) {
            used[i] = true;
        }
    };

    // Implement logic to detect exact matches of length 10-20, while avoiding double-counting
    for (size_t i = 0; i < n1; ++i) {
        for (size_t j = 0; j < n2; ++j) {
            int matchLength = 0;
            // Look for matches starting at i in submission1 and j in submission2
            while (i + matchLength < n1 && j + matchLength < n2 &&
                   submission1[i + matchLength] == submission2[j + matchLength] &&
                   matchLength < 20) {
                ++matchLength;
            }

            if (matchLength >= 10 && matchLength <= 20) {
                // Check if this match overlaps with any previously found match
                bool overlap = false;
                for (int k = 0; k < matchLength; ++k) {
                    if (used1[i + k] || used2[j + k]) {
                        overlap = true;
                        break;
                    }
                }

                // If no overlap, count this match and mark indices as used
                if (!overlap) {
                    totalMatchLength += matchLength; // Add to total match length
                    mark_used(used1, i, matchLength);
                    mark_used(used2, j, matchLength);
                }
            }
        }
    }
    // Step 2: Find the longest approximate match for sequences of length >= 30
    for (size_t i = 0; i <= n1 - 30; ++i) {
        for (size_t j = 0; j <= n2 - 30; ++j) {
            int length = 0;
            int matchLength = 0;
            while (i + length < n1 && j + length < n2) {
                if(submission1[i+length] == submission2[j+length]) matchLength++;
                length++;
                if(length >=30 && matchLength>= 0.8*length && matchLength > longestApproxMatch){
                    longestApproxMatch = matchLength;
                    longestApproxMatchStart1 = i;
                    longestApproxMatchStart2 = j;
                }
                // If no further approx match could be found
                bool c1 = matchLength+n1-i-1-length < 0.8 *(n1-i);
                bool c2 = matchLength+n2-j-1-length < 0.8 *(n2-j);
                if(c1 || c2) break;
                // If no longer approx match could be found
                int check1 = matchLength+n1-i-length-1;
                int check2 = matchLength+n2-j-length-1;
                if(std::min(check1,check2) < longestApproxMatch) break;
            }
        }
    }

    // Step 3: Set results based on findings
    result[1] = totalMatchLength;  // Total length of exact matches
    result[2] = longestApproxMatch; // Longest approximate match length
    result[3] = longestApproxMatchStart1; // Start index in submission1
    result[4] = longestApproxMatchStart2; // Start index in submission2

    // Determine plagiarism flag based on thresholds
    if (totalMatchLength > 0.4*(std::min(n1,n2)) || longestApproxMatch > 0.3*(std::min(n1,n2))) {
        result[0] = 1;  // Flag as plagiarized
    }

    return result;
}