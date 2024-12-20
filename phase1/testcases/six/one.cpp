#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>

std::unordered_map<int, long long> memo;

long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    if (memo.find(n) != memo.end()) {
        return memo[n];
    }
    memo[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return memo[n];
}

void generateAndPrintFibonacciSeries(int terms) {
    std::cout << "Fibonacci Series up to " << terms << " terms:" << std::endl;
    for (int i = 0; i < terms; ++i) {
        std::cout << "Fibonacci[" << i + 1 << "] = " << std::setw(10) << fibonacci(i) << std::endl;
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

    generateAndPrintFibonacciSeries(terms);

    return 0;
}
