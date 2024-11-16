

#include "solution.h"


ComplexNumber::ComplexNumber(double real, double imaginary) : real(real), imaginary(imaginary) {}

ComplexNumber ComplexNumber::add(const ComplexNumber& a, const ComplexNumber& b) const {
    double new_real = a.real + b.real;
    double new_imaginary = a.imaginary + b.imaginary;
    return {new_real, new_imaginary};
}

ComplexNumber ComplexNumber::sub(const ComplexNumber& a, const ComplexNumber& b) const{
    double new_real = a.real - b.real;
    double new_imaginary = a.imaginary - b.imaginary;
    return {new_real, new_imaginary};
}

ComplexNumber ComplexNumber::multiply(const ComplexNumber& a, const ComplexNumber& b) const{
    double new_real = a.real * b.real - a.imaginary * b.imaginary;
    double new_imaginary = a.real * b.imaginary +  a.imaginary * b.real;
    return {new_real, new_imaginary};
}

ComplexNumber ComplexNumber::division(const ComplexNumber& a, const ComplexNumber& b)  const{
    double denominator = b.real * b.real + b.imaginary * b.imaginary;

    double new_real = (a.real * b.real + a.imaginary * b.imaginary) / denominator;
    double new_imaginary = (a.imaginary * b.real - a.real * b.imaginary) / denominator;
    return {new_real, new_imaginary};
}

double ComplexNumber::absolute(const ComplexNumber& a)  const{
    return std::sqrt(a.real * a.real + a.imaginary * a.imaginary);
}

double ComplexNumber::argument(const ComplexNumber& a)  const{
    return std::atan2(a.imaginary, a.real);
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber &other) const {
    return add(*this, other);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber &other) const {
    return sub(*this, other);
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber &other) const {
    return multiply(*this, other);
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber &other) const {
    return division(*this, other);
}

bool ComplexNumber::operator==(const ComplexNumber &other) const {
    return this->real == other.real && this->imaginary == other.imaginary;
}

double ComplexNumber::get_real() const {
    return this->real;
}

double ComplexNumber::get_imaginary() const {
    return this->imaginary;
}

ComplexNumber::~ComplexNumber() = default;
