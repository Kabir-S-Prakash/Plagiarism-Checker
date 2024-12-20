#include <iostream>
#include <iomanip>

struct Matrix2x2 {
    long long m[2][2];
    
    Matrix2x2() {
        m[0][0] = 1; m[0][1] = 1;
        m[1][0] = 1; m[1][1] = 0;
    }

    Matrix2x2 operator*(const Matrix2x2& other) const {
        Matrix2x2 result;
        result.m[0][0] = m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0];
        result.m[0][1] = m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1];
        result.m[1][0] = m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0];
        result.m[1][1] = m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1];
        return result;
    }
};

Matrix2x2 matrixPower(Matrix2x2 base, int exp) {
    Matrix2x2 result;
    result.m[0][0] = 1; result.m[0][1] = 0;
    result.m[1][0] = 0; result.m[1][1] = 1;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp /= 2;
    }
    return result;
}

long long fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    Matrix2x2 base;
    Matrix2x2 result = matrixPower(base, n - 1);
    return result.m[0][0];
}

void printFibonacciSeries(int terms) {
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

    printFibonacciSeries(terms);

    return 0;
}
