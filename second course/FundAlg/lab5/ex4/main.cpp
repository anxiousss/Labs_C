#include "solution.h"
#include <iostream>
#include <cmath>

void testAddition() {
    ComplexNumber a(3.0, 4.0);
    ComplexNumber b(1.0, 2.0);
    ComplexNumber result = a + b;
    if (result.get_real() == 4.0 && result.get_imaginary() == 6.0) {
        std::cout << "Addition test passed!" << std::endl;
    } else {
        std::cout << "Addition test failed! Expected 4.0 + 6.0i, got " << result.get_real() << " + " << result.get_imaginary() << "i" << std::endl;
    }
}

void testSubtraction() {
    ComplexNumber a(3.0, 4.0);
    ComplexNumber b(1.0, 2.0);
    ComplexNumber result = a - b;
    if (result.get_real() == 2.0 && result.get_imaginary() == 2.0) {
        std::cout << "Subtraction test passed!" << std::endl;
    } else {
        std::cout << "Subtraction test failed! Expected 2.0 + 2.0i, got " << result.get_real() << " + " << result.get_imaginary() << "i" << std::endl;
    }
}

void testMultiplication() {
    ComplexNumber a(3.0, 4.0);
    ComplexNumber b(1.0, 2.0);
    ComplexNumber result = a * b;
    if (result.get_real() == -5.0 && result.get_imaginary() == 10.0) {
        std::cout << "Multiplication test passed!" << std::endl;
    } else {
        std::cout << "Multiplication test failed! Expected -5.0 + 10.0i, got " << result.get_real() << " + " << result.get_imaginary() << "i" << std::endl;
    }
}

void testDivision() {
    ComplexNumber a(3.0, 4.0);
    ComplexNumber b(1.0, 2.0);
    ComplexNumber result = a / b;
    if (std::abs(result.get_real() - 2.2) < 1e-9 && std::abs(result.get_imaginary() + 0.4) < 1e-9) {
        std::cout << "Division test passed!" << std::endl;
    } else {
        std::cout << "Division test failed! Expected 2.2 - 0.4i, got " << result.get_real() << " + " << result.get_imaginary() << "i" << std::endl;
    }
}

void testAbsolute() {
    ComplexNumber a(3.0, 4.0);
    double result = a.absolute(a);
    if (result == 5.0) {
        std::cout << "Absolute test passed!" << std::endl;
    } else {
        std::cout << "Absolute test failed! Expected 5.0, got " << result << std::endl;
    }
}

void testArgument() {
    ComplexNumber a(3.0, 4.0);
    double result = a.argument(a);
    if (std::abs(result - std::atan2(4.0, 3.0)) < 1e-9) {
        std::cout << "Argument test passed!" << std::endl;
    } else {
        std::cout << "Argument test failed! Expected " << std::atan2(4.0, 3.0) << ", got " << result << std::endl;
    }
}

void testEquality() {
    ComplexNumber a(3.0, 4.0);
    ComplexNumber b(3.0, 4.0);
    ComplexNumber c(1.0, 2.0);
    if (a == b && !(a == c)) {
        std::cout << "Equality test passed!" << std::endl;
    } else {
        std::cout << "Equality test failed!" << std::endl;
    }
}

int main() {
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testAbsolute();
    testArgument();
    testEquality();

    std::cout << "All tests completed!" << std::endl;
    return 0;
}
