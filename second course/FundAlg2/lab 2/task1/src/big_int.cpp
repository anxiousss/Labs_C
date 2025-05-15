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

BigInt::BigInt(const std::string& str, unsigned int base) : base(base) {

    if (base % 10 != 0) {
        throw std::invalid_argument("base must be power of 10\n");
    }

    isNegative = false;
    size_t start = 0;

    if (!str.empty() && str[0] == '-') {
        isNegative = true;
        start = 1;
    }

    std::string num_str = str.substr(start);
    digits.clear();

    int chunk_size = base == 10 ? 1 : static_cast<int>(log10(base));
    size_t total_digits = num_str.length();
    size_t padding = (chunk_size - (total_digits % chunk_size)) % chunk_size;
    num_str = std::string(padding, '0') + num_str;

    for (int pos = num_str.length(); pos > 0; pos -= chunk_size) {
        int start_pos = std::max(0, pos - chunk_size);
        std::string chunk = num_str.substr(start_pos, pos - start_pos);
        unsigned long long digit = std::stoull(chunk);
        digits.push_back(digit);
    }

    removeLeadingZeros();
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
} //LCOV_EXCL_LINE

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
} //LCOV_EXCL_LINE



BigInt BigInt::operator-(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = BigInt::convertToBase(*this, 10);
        BigInt b = BigInt::convertToBase(other, 10);
        return a - b;
    }

    BigInt temp = other;
    temp.isNegative = !temp.isNegative;
    return *this + temp;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = convertToBase(*this, 10);
        BigInt b = convertToBase(other, 10);
        return a * b;
    }

    BigInt result(0, base);
    result.digits.resize(digits.size() + other.digits.size(), 0);
    result.isNegative = isNegative != other.isNegative;

    for (size_t i = 0; i < digits.size(); ++i) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            size_t idx = i + j;
            if (idx >= result.digits.size()) {
                result.digits.resize(idx + 1, 0);
            }

            unsigned long long product = digits[i] * (j < other.digits.size() ? other.digits[j] : 0);
            unsigned long long total = result.digits[idx] + product + carry;

            carry = total / base;
            result.digits[idx] = total % base;
        }
    }

    result.removeLeadingZeros();
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (base != other.base) {
        BigInt a = BigInt::convertToBase(*this, 10);
        BigInt b = BigInt::convertToBase(other, 10);
        return a / b;
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
} //LCOV_EXCL_LINE

BigInt& BigInt::operator--() {
    *this -= BigInt(1, base);
    return *this;
} //LCOV_EXCL_LINE

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --*this;
    return temp;
} //LCOV_EXCL_LINE

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

BigInt BigInt::convertToBase(const BigInt& num, unsigned int new_base) {
    BigInt result(0, new_base);
    BigInt power(1, new_base);

    for (unsigned long long digit : num.digits) {
        BigInt term = BigInt(digit, new_base) * power;
        result += term;
        power = power * BigInt(num.base, new_base);
    }

    result.isNegative = num.isNegative;
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
