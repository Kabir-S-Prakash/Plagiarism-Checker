#include <iostream>
#include <vector>
#include <iomanip>

std::vector<long long> generateFibonacci(int terms) {
    std::vector<long long> fibonacciSeries;
    long long first = 0, second = 1, next;
    
    for (int i = 0; i < terms; ++i) {
        if (i <= 1) {
            next = i;
        } else {
            next = first + second;
            first = second;
            second = next;
        }
        fibonacciSeries.push_back(next);
    }
    return fibonacciSeries;
}

void printFibonacciSeries(const std::vector<long long>& series) {
    for (size_t i = 0; i < series.size(); ++i) {
        std::cout << "Fibonacci[" << i + 1 << "] = " << std::setw(10) << series[i] << std::endl;
    }
}

int main() {
    int terms;
    std::cout << "Enter the number of terms for the Fibonacci series: ";
    std::cin >> terms;

    if (terms <= 0) {
        std::cerr << "Number of terms must be positive!" << std::endl;
        return 1;
    }

    std::vector<long long> fibonacciSeries = generateFibonacci(terms);
    std::cout << "Generated Fibonacci series up to " << terms << " terms:" << std::endl;
    printFibonacciSeries(fibonacciSeries);

    return 0;
}
