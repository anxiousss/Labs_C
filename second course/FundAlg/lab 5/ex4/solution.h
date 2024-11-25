
#ifndef EX5_4_SOLUTION_H
#define EX5_4_SOLUTION_H

#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

class div_by_null : std::exception {};

class ComplexNumber {
private:
    std::pair<double, double> values;

public:
    ComplexNumber(double real = 0.0, double imaginary = 0.0);

    std::pair<double, double> get_values() const;

    double absolute() const;

    double argument() const;

    ComplexNumber operator+(const ComplexNumber& other) const;

    ComplexNumber& operator+=(const ComplexNumber& other);

    void add(const ComplexNumber& n);

    ComplexNumber operator-(const ComplexNumber& other) const;

    ComplexNumber& operator-=(const ComplexNumber& other);

    void sub(const ComplexNumber& n);

    ComplexNumber operator*(const ComplexNumber& other) const;

    ComplexNumber& operator*=(const ComplexNumber& other);

    void multiply(const ComplexNumber& n);

    ComplexNumber operator/(const ComplexNumber& other) const;

    ComplexNumber& operator/=(const ComplexNumber& other);

    void divide(const ComplexNumber& n);

    bool operator==(const ComplexNumber& other) const;

    ComplexNumber operator-() const;

    ~ComplexNumber();
};

std::ostream& operator<<(std::ostream& out, const ComplexNumber& n);


#endif //EX5_4_SOLUTION_H
