
#ifndef EX5_4_SOLUTION_H
#define EX5_4_SOLUTION_H

#include <iostream>
#include <cmath>

class ComplexNumber {
private:
    double real;
    double imaginary;

public:
    ComplexNumber(double real, double imaginary);

    double get_real() const;

    double get_imaginary() const;

    ComplexNumber add(const ComplexNumber& a, const ComplexNumber& b) const;

    ComplexNumber sub(const ComplexNumber& a, const ComplexNumber& b) const;

    ComplexNumber multiply(const ComplexNumber& a, const ComplexNumber& b) const;

    ComplexNumber division(const ComplexNumber& a, const ComplexNumber& b) const;

    double absolute(const ComplexNumber& a) const;

    double argument(const ComplexNumber& a) const;

    ComplexNumber operator+(const ComplexNumber& other) const;

    ComplexNumber operator-(const ComplexNumber& other) const;

    ComplexNumber operator*(const ComplexNumber& other) const;

    ComplexNumber operator/(const ComplexNumber& other) const;

    bool operator==(const ComplexNumber& other) const;

    ~ComplexNumber();

};

#endif //EX5_4_SOLUTION_H
