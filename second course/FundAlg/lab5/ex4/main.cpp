#include "solution.h"
#include <iostream>
#include <cmath>

void testConstructor() {
    ComplexNumber c(3.0, 4.0);
    auto values = c.get_values();
    if (values.first == 3.0 && values.second == 4.0) {
        std::cout << "Test Constructor: PASSED" << std::endl;
    } else {
        std::cout << "Test Constructor: FAILED" << std::endl;
    }
}

void testAbsolute() {
    ComplexNumber c(3.0, 4.0);
    double absValue = c.absolute(c);
    if (absValue == 5.0) {
        std::cout << "Test Absolute: PASSED" << std::endl;
    } else {
        std::cout << "Test Absolute: FAILED" << std::endl;
    }
}

void testArgument() {
    ComplexNumber c(1.0, 1.0);
    double argValue = c.argument(c);
    if (argValue == M_PI / 4) {
        std::cout << "Test Argument: PASSED" << std::endl;
    } else {
        std::cout << "Test Argument: FAILED" << std::endl;
    }
}

void testAddition() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(3.0, 4.0);
    ComplexNumber result = c1 + c2;
    auto values = result.get_values();
    if (values.first == 4.0 && values.second == 6.0) {
        std::cout << "Test Addition: PASSED" << std::endl;
    } else {
        std::cout << "Test Addition: FAILED" << std::endl;
    }
}

void testSubtraction() {
    ComplexNumber c1(5.0, 7.0);
    ComplexNumber c2(2.0, 3.0);
    ComplexNumber result = c1 - c2;
    auto values = result.get_values();
    if (values.first == 3.0 && values.second == 4.0) {
        std::cout << "Test Subtraction: PASSED" << std::endl;
    } else {
        std::cout << "Test Subtraction: FAILED" << std::endl;
    }
}

void testMultiplication() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(3.0, 4.0);
    ComplexNumber result = c1 * c2;
    auto values = result.get_values();
    if (values.first == -5.0 && values.second == 10.0) {
        std::cout << "Test Multiplication: PASSED" << std::endl;
    } else {
        std::cout << "Test Multiplication: FAILED" << std::endl;
    }
}

void testDivision() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(3.0, 4.0);
    ComplexNumber result = c1 / c2;
    auto values = result.get_values();
    if (std::abs(values.first - 0.44) < 1e-2 && std::abs(values.second - 0.08) < 1e-2) {
        std::cout << "Test Division: PASSED" << std::endl;
    } else {
        std::cout << "Test Division: FAILED" << std::endl;
    }
}

void testEquality() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(1.0, 2.0);
    ComplexNumber c3(1.0, 2.000001);
    if (c1 == c2 && !(c1 == c3)) {
        std::cout << "Test Equality: PASSED" << std::endl;
    } else {
        std::cout << "Test Equality: FAILED" << std::endl;
    }
}

void testAdditionAssignment() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(3.0, 4.0);
    c1 += c2;
    auto values = c1.get_values();
    if (values.first == 4.0 && values.second == 6.0) {
        std::cout << "Test Addition Assignment: PASSED" << std::endl;
    } else {
        std::cout << "Test Addition Assignment: FAILED" << std::endl;
    }
}

void testSubtractionAssignment() {
    ComplexNumber c1(5.0, 7.0);
    ComplexNumber c2(2.0, 3.0);
    c1 -= c2;
    auto values = c1.get_values();
    if (values.first == 3.0 && values.second == 4.0) {
        std::cout << "Test Subtraction Assignment: PASSED" << std::endl;
    } else {
        std::cout << "Test Subtraction Assignment: FAILED" << std::endl;
    }
}

void testMultiplicationAssignment() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(3.0, 4.0);
    c1 *= c2;
    auto values = c1.get_values();
    if (values.first == -5.0 && values.second == 10.0) {
        std::cout << "Test Multiplication Assignment: PASSED" << std::endl;
    } else {
        std::cout << "Test Multiplication Assignment: FAILED" << std::endl;
    }
}

void testDivisionAssignment() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber c2(3.0, 4.0);
    c1 /= c2;
    auto values = c1.get_values();
    if (std::abs(values.first - 0.44) < 1e-2 && std::abs(values.second - 0.08) < 1e-2) {
        std::cout << "Test Division Assignment: PASSED" << std::endl;
    } else {
        std::cout << "Test Division Assignment: FAILED" << std::endl;
    }
}

void testUnaryMinus() {
    ComplexNumber c1(1.0, 2.0);
    ComplexNumber result = -c1;
    auto values = result.get_values();
    if (values.first == -1.0 && values.second == -2.0) {
        std::cout << "Test Unary Minus: PASSED" << std::endl;
    } else {
        std::cout << "Test Unary Minus: FAILED" << std::endl;
    }
}

int main() {
    testConstructor();
    testAbsolute();
    testArgument();
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testEquality();
    testAdditionAssignment();
    testSubtractionAssignment();
    testMultiplicationAssignment();
    testDivisionAssignment();
    testUnaryMinus();

    return 0;
}