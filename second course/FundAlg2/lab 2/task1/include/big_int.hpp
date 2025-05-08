#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

class BigInt {
public:
    BigInt();
    explicit BigInt(long long value, unsigned int base = 10);
    explicit BigInt(const std::string& str, unsigned int base = 10);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    ~BigInt() = default;

    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);

    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);

    bool operator==(const BigInt& other) const;
    std::strong_ordering operator<=>(const BigInt& other) const;


    [[nodiscard]] unsigned int getBase() const { return base; }
    [[nodiscard]] bool getNegative() const { return isNegative;}

    friend std::istream& operator>>(std::istream& is, BigInt& num);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);


private:
    std::vector<unsigned long long> digits;
    bool isNegative;
    unsigned int base;

    void removeLeadingZeros();
    [[nodiscard]] int compareMagnitude(const BigInt& other) const;

    [[nodiscard]] BigInt absoluteAdd(const BigInt& other) const;
    [[nodiscard]] BigInt absoluteSubtract(const BigInt& other) const;

    [[nodiscard]] std::pair<BigInt, BigInt> divide(const BigInt& divisor) const;

    static BigInt convertToBase(const BigInt& bigInt, unsigned int newBase);
};