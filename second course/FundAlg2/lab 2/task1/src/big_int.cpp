#include "../include/big_int.hpp"
#include <cctype>
#include <stdexcept>
#include <climits>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <iomanip>

BigInt::BigInt() : isNegative(false), base(10) {
    digits.push_back(0);
}

//основа должна быть степенью 10
BigInt::BigInt(long long value, unsigned int base) : base(base) {
    isNegative = (value < 0);

    unsigned long long abs_value;
    if (value == LLONG_MIN) {
        abs_value = static_cast<unsigned long long>(LLONG_MAX) + 1;
    } else {
        abs_value = std::abs(value);
    }

    if (abs_value == 0) {
        digits.push_back(0);
    } else {
        while (abs_value > 0) {
            digits.push_back(abs_value % base);
            abs_value /= base;
        }
    }

    if (digits.empty()) {
        digits.push_back(0);
        isNegative = false;
    }
}


BigInt::BigInt(const std::string &str, unsigned int base): base(base) {
    auto it = str.rbegin();
    if (str.at(0) == '-') {
        isNegative = true;
        ++it;
    }


    if (str == "0") {
        digits.push_back(0);
    } else {
        for (;it != str.rend(); it++) {
            digits.push_back((static_cast<int>(*it) - 48) % base);
        }
    }

    if (digits.empty()) {
        digits.push_back(0);
        isNegative = false;
    }


}



BigInt::BigInt(const BigInt& other) = default;

BigInt::BigInt(BigInt&& other) noexcept
        : digits(std::move(other.digits)),
          isNegative(other.isNegative),
          base(other.base) {}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        digits = other.digits;
        isNegative = other.isNegative;
        base = other.base;
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
        isNegative = other.isNegative;
        base = other.base;
    }
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (base != other.base) {
        throw std::invalid_argument("Bases must be the same");
    }

    BigInt result(0, base);
    if (isNegative == other.isNegative) {
        result = absoluteAdd(other);
        result.isNegative = isNegative;
    } else {
        int cmp = compareMagnitude(other);
        if (cmp == 0) {
            return BigInt(0, base);
        } else if (cmp > 0) {
            result = absoluteSubtract(other);
            result.isNegative = isNegative;
        } else {
            result = other.absoluteSubtract(*this);
            result.isNegative = other.isNegative;
        }
    }

    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::absoluteAdd(const BigInt& other) const {
    BigInt result(0, base);
    size_t maxSize = std::max(digits.size(), other.digits.size());
    unsigned long long carry = 0;

    for (size_t i = 0; i < maxSize || carry; ++i) {
        unsigned long long sum = carry;
        if (i < digits.size()) sum += digits[i];
        if (i < other.digits.size()) sum += other.digits[i];
        carry = sum / base;
        result.digits.push_back(sum % base);
    }

    return result;
}

BigInt BigInt::absoluteSubtract(const BigInt& other) const {
    BigInt result(0, base);
    unsigned long long borrow = 0;

    for (size_t i = 0; i < digits.size(); ++i) {
        long long diff = digits[i] - borrow;
        borrow = 0;

        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }

        if (diff < 0) {
            diff += base;
            borrow = 1;
        }

        result.digits.push_back(diff);
    }

    result.removeLeadingZeros();
    return result;
}



BigInt BigInt::operator-(const BigInt& other) const {
    if (base != other.base) {
        throw std::invalid_argument("Bases must be the same for subtraction");
    }

    BigInt temp = other;
    temp.isNegative = !temp.isNegative;
    return *this + temp;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (base != other.base) {
        throw std::invalid_argument("Bases must be the same for multiplication");
    }

    BigInt result(0, base);
    result.digits.resize(digits.size() + other.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            unsigned long long current = result.digits[i + j] + carry +
                                         digits[i] * (j < other.digits.size() ? other.digits[j] : 0);
            carry = current / base;
            result.digits[i + j] = current % base;
        }
    }

    result.isNegative = isNegative != other.isNegative;
    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    auto [quotient, remainder] = divide(other);
    return quotient;
}

BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;
}

BigInt& BigInt::operator++() {
    *this += BigInt(1, base);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++*this;
    return temp;
}

BigInt& BigInt::operator--() {
    *this -= BigInt(1, base);
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --*this;
    return temp;
}

bool BigInt::operator==(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(10);
        BigInt b = other.convertToBase(10);
        return a == b;
    }
    return isNegative == other.isNegative && digits == other.digits;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(10);
        BigInt b = other.convertToBase(10);
        return a < b;
    }

    if (isNegative != other.isNegative)
        return isNegative;

    if (isNegative)
        return compareMagnitude(other) > 0;

    return compareMagnitude(other) < 0;
}
bool BigInt::operator>(const BigInt& other) const {
    return !(*this < other) && !(*this == other);
}

bool BigInt::operator<=(const BigInt& other) const {
    return (*this < other) || (*this == other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return (*this > other) || (*this == other);
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    std::stringstream output;
    if (num.isNegative) {
        output << "-";
    }
    for (auto it = num.digits.rbegin(); it != num.digits.rend(); ++it) {
        output << *it;
    }
    std::string s;
    output >> s;
    os << s;
    return os;
}

void BigInt::removeLeadingZeros() {
    while (digits.size() > 1 && digits.front() == 0) {
        digits.erase(digits.begin());
    }
    if (digits.empty()) {
        digits.push_back(0);
        isNegative = false;
    }
}



int BigInt::compareMagnitude(const BigInt& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size() ? 1 : -1;
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] > other.digits[i] ? 1 : -1;
        }
    }
    return 0;
}

BigInt BigInt::add(const BigInt& other) const {
    BigInt result(0, base);
    size_t maxSize = std::max(digits.size(), other.digits.size());
    unsigned long long carry = 0;

    for (size_t i = 0; i < maxSize || carry; ++i) {
        unsigned long long sum = carry;
        if (i < digits.size()) sum += digits[i];
        if (i < other.digits.size()) sum += other.digits[i];

        carry = sum / base;
        result.digits.push_back(sum % base);
    }

    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::subtract(const BigInt& other) const {
    BigInt result(0, base);
    result.digits.resize(digits.size(), 0);
    unsigned long long borrow = 0;

    for (size_t i = 0; i < digits.size(); ++i) {
        long long diff = digits[i] - borrow;
        if (i < other.digits.size()) diff -= other.digits[i];
        borrow = 0;
        if (diff < 0) {
            diff += base;
            borrow = 1;
        }
        result.digits[i] = diff;
    }

    result.removeLeadingZeros();
    return result;
}

std::pair<BigInt, BigInt> BigInt::divide(const BigInt& divisor) const {
    if (divisor == BigInt(0, base)) {
        throw std::invalid_argument("Division by zero");
    }

    BigInt quotient(0, base);
    BigInt remainder = *this;
    remainder.isNegative = false;

    BigInt absDivisor = divisor;
    absDivisor.isNegative = false;

    if (remainder < absDivisor) {
        return {BigInt(0, base), remainder};
    }

    BigInt current(0, base);
    quotient.digits.resize(digits.size(), 0);

    for (int i = digits.size() - 1; i >= 0; --i) {
        current = current * BigInt(base, base) + BigInt(digits[i], base);
        unsigned long long l = 0, r = base;
        while (l < r) {
            unsigned long long m = (l + r + 1) / 2;
            BigInt temp = absDivisor * BigInt(m, base);
            if (temp <= current) {
                l = m;
            } else {
                r = m - 1;
            }
        }
        quotient.digits[i] = l;
        current = current - absDivisor * BigInt(l, base);
    }

    quotient.isNegative = isNegative != divisor.isNegative;
    quotient.removeLeadingZeros();
    current.isNegative = isNegative;
    return {quotient, current};
}

unsigned char BigInt::digitToChar(unsigned long long digit) {
    if (digit < 10) {
        return '0' + digit;
    } else {
        return 'A' + (digit - 10);
    }
}

unsigned long long BigInt::charToDigit(char c, unsigned int base) {
    int value;
    if (isdigit(c)) {
        value = c - '0';
    } else if (isupper(c)) {
        value = 10 + c - 'A';
    } else if (islower(c)) {
        value = 10 + c - 'a';
    } else {
        throw std::invalid_argument("Invalid character");
    }

    if (value < 0 || value >= static_cast<int>(base)) {
        throw std::invalid_argument("Digit out of range for base");
    }

    return value;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string str;
    is >> str;
    num = BigInt(str, num.getBase());
    return is;
}

BigInt BigInt::convertToBase(unsigned int newBase) const {
    if (newBase == base) return *this;

    BigInt result(0, newBase);
    BigInt power(1, newBase);

    for (unsigned long long i : digits) {
        BigInt digit(i, newBase);
        result += digit * power;
        power *= BigInt(base, newBase);
    }

    result.isNegative = isNegative;
    return result;
}