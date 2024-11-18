#include "solution.h"

ComplexNumber::ComplexNumber(double real, double imaginary) : values{real, imaginary} {}

std::pair<double, double> ComplexNumber::get_values() const {
    return this->values;
}

double ComplexNumber::absolute(const ComplexNumber& a) const {
    return std::sqrt(a.values.first * a.values.first + a.values.second * a.values.second);
}

double ComplexNumber::argument(const ComplexNumber& a) const {
    return std::atan2(a.values.second, a.values.first);
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber &other) const {
    auto tmp = other.get_values();
    return {this->values.first + tmp.first, this->values.second + tmp.second};
}

ComplexNumber &ComplexNumber::operator+=(const ComplexNumber &other) {
    auto tmp = other.get_values();
    this->values.first += tmp.first;
    this->values.second += tmp.second;
    return *this;
}

void ComplexNumber::add(const ComplexNumber &n) {
    *this += n;
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber &other) const {
    auto tmp = other.get_values();
    return {this->values.first - tmp.first, this->values.second - tmp.second};
}

ComplexNumber ComplexNumber::operator-=(const ComplexNumber &other) {
    auto tmp = other.get_values();
    this->values.first -= tmp.first;
    this->values.second -= tmp.second;
    return *this;
}

void ComplexNumber::sub(const ComplexNumber &n) {
    *this -= n;
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber &other) const {
    auto tmp = other.get_values();
    double new_real = this->values.first * tmp.first - this->values.second * tmp.second;
    double new_imaginary = this->values.first * tmp.second + this->values.second * tmp.first;
    return {new_real, new_imaginary};
}

ComplexNumber ComplexNumber::operator*=(const ComplexNumber &other) {
    auto tmp = other.get_values();
    double new_real = this->values.first * tmp.first - this->values.second * tmp.second;
    double new_imaginary = this->values.first * tmp.second + this->values.second * tmp.first;
    this->values.first = new_real;
    this->values.second = new_imaginary;
    return *this;
}

void ComplexNumber::multiply(const ComplexNumber &n) {
    *this *= n;
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber &other) const {
    auto tmp = other.get_values();
    double denominator = tmp.first * tmp.first + tmp.second * tmp.second;
    double new_real = (this->values.first * tmp.first + this->values.second * tmp.second) / denominator;
    double new_imaginary = (this->values.second * tmp.first - this->values.first * tmp.second) / denominator;
    return {new_real, new_imaginary};
}

ComplexNumber ComplexNumber::operator/=(const ComplexNumber &other) {
    auto tmp = other.get_values();
    double denominator = tmp.first * tmp.first + tmp.second * tmp.second;
    double new_real = (this->values.first * tmp.first + this->values.second * tmp.second) / denominator;
    double new_imaginary = (this->values.second * tmp.first - this->values.first * tmp.second) / denominator;
    this->values.first = new_real;
    this->values.second = new_imaginary;
    return *this;
}

void ComplexNumber::divide(const ComplexNumber &n) {
    *this /= n;
}

bool ComplexNumber::operator==(const ComplexNumber &other) const {
    auto tmp = other.get_values();
    double eps = 1e-6;
    return (fabs(this->values.first - tmp.first) < eps && fabs(this->values.second - tmp.second) < eps);
}

ComplexNumber ComplexNumber::operator-() const {
    return {-this->values.first, -this->values.second};
}

ComplexNumber::~ComplexNumber() = default;