#include "../include/big_int.hpp"
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
    if (base % 10 != 0) {
        throw std::invalid_argument("base must be power of 10\n");
    }

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
    } else {
        isNegative = false;
    }



    if (str == "0") {
        digits.push_back(0);
    } else {
        if (isNegative) {
            for (; it != str.rend() - 1; it++) {
                digits.push_back((static_cast<int>(*it) - 48) % base);
            }
        } else {
            for (; it != str.rend(); it++) {
                digits.push_back((static_cast<int>(*it) - 48) % base);
            }
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
        BigInt a = BigInt::convertToBase(*this, 10);
        BigInt b = BigInt::convertToBase(other, 10);
        return a + b;
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

    result.digits.clear();
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
    result.digits.clear();
    for (size_t i = 0; i < digits.size(); ++i) {
        long long diff = (long long)digits[i] - (long long )borrow;
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
        BigInt b = BigInt::convertToBase(other, this->base);
        return *this - b;    }

    BigInt temp = other;
    temp.isNegative = !temp.isNegative;
    return *this + temp;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (base != other.base) {
        BigInt b = BigInt::convertToBase(other, this->base);
        return *this * b;    }

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
    if (base != other.base) {
        BigInt b = BigInt::convertToBase(other, this->base);
        return *this / b;
    }
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
        BigInt b = BigInt::convertToBase(other, this->base);
        return *this == b;
    }
    return isNegative == other.isNegative && digits == other.digits;
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
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
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
        current = current * BigInt(base, base) + BigInt((long long)digits[i], base);
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


std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string str;
    is >> str;
    num = BigInt(str, num.getBase());
    return is;
}

BigInt BigInt::convertToBase(const BigInt& bigInt, unsigned int newBase) {
    if (newBase == bigInt.base) {
        return bigInt;
    }
    BigInt result(0, newBase);
    unsigned long long value = 0;
    int index = 0;
    for (auto d: bigInt.digits) {
        value += d * (unsigned long long)pow(bigInt.base, index);
        ++index;
    }
    if (value == 0) {
        return result;
    }

    result.digits.clear();
    while (value > 0) {
        result.digits.push_back(value % newBase);
        value /= newBase;
    }

    result.isNegative = bigInt.isNegative;
    return result;
}

std::strong_ordering BigInt::operator<=>(const BigInt &other) const {
    if (isNegative != other.isNegative) {
        return isNegative < other.isNegative ? std::strong_ordering::greater : std::strong_ordering::less;
    }

    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size() ? std::strong_ordering::greater : std::strong_ordering::less;
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] > other.digits[i] ? std::strong_ordering::greater : std::strong_ordering::less;
        }
    }
    return std::strong_ordering::equivalent;
}
