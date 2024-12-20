#include <iostream>
#include <vector>
#include <iomanip>

std::vector<long long> fibonacciSeries(int numTerms) {
    std::vector<long long> series;
    long long firstTerm = 0, secondTerm = 1, nextTerm;

    for (int i = 0; i < numTerms; ++i) {
        if (i == 0) {
            nextTerm = firstTerm;
        } else if (i == 1) {
            nextTerm = secondTerm;
        } else {
            nextTerm = firstTerm + secondTerm;
            firstTerm = secondTerm;
            secondTerm = nextTerm;
        }
        series.push_back(nextTerm);
    }
    return series;
}

void displaySeries(const std::vector<long long>& series) {
    for (size_t i = 0; i < series.size(); ++i) {
        std::cout << "Fibonacci[" << i + 1 << "] = " << std::setw(10) << series[i] << std::endl;
    }
}

int main() {
    int numTerms;
    std::cout << "Enter the number of terms for the Fibonacci series: ";
    std::cin >> numTerms;

    if (numTerms <= 0) {
        std::cerr << "Number of terms must be positive!" << std::endl;
        return 1;
    }

    std::vector<long long> series = fibonacciSeries(numTerms);
    std::cout << "Generated Fibonacci series up to " << numTerms << " terms:" << std::endl;
    displaySeries(series);

    return 0;
}
