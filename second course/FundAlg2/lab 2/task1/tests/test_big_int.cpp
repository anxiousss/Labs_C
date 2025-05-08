#include <gtest/gtest.h>
#include "../include/big_int.hpp"

TEST(BigIntTest, DefaultConstructor) {
    BigInt num;
    EXPECT_EQ(num.getBase(), 10);
    EXPECT_EQ(num, BigInt(0));
}

TEST(BigIntTest, LongLongConstructor) {
    BigInt numPos(12345, 10);
    EXPECT_EQ(numPos, BigInt("12345", 10));

    BigInt numNeg(-12345, 10);
    EXPECT_EQ(numNeg, BigInt("-12345", 10));

    BigInt numZero(0, 10);
    EXPECT_EQ(numZero, BigInt());
}

TEST(BigIntTest, StringConstructor) {
    BigInt numPos("12345");
    EXPECT_EQ(numPos, BigInt(12345));

    BigInt numNeg("-12345");
    EXPECT_EQ(numNeg, BigInt(-12345));

    BigInt numZero("0");
    EXPECT_EQ(numZero, BigInt(0));

    EXPECT_THROW(BigInt("12a45", 10), std::invalid_argument);
}

TEST(BigIntTest, BaseValidation) {
    EXPECT_THROW(BigInt(123, 15), std::invalid_argument);
    EXPECT_NO_THROW(BigInt(123, 100));
}

TEST(BigIntTest, Addition) {
    BigInt a(123, 10);
    BigInt b(456, 10);
    EXPECT_EQ(a + b, BigInt(579));

    BigInt c("-123", 10);
    BigInt d("-456", 10);
    EXPECT_EQ(c + d, BigInt(-579));

    BigInt e(999, 10);
    BigInt f(1, 10);
    EXPECT_EQ(e + f, BigInt(1000));
}

TEST(BigIntTest, Subtraction) {
    BigInt a(456, 10);
    BigInt b(123, 10);
    EXPECT_EQ(a - b, BigInt(333));

    BigInt c(123, 10);
    BigInt d(456, 10);
    EXPECT_EQ(c - d, BigInt(-333));

    BigInt e(1000, 10);
    BigInt f(1, 10);
    EXPECT_EQ(e - f, BigInt(999));
}

TEST(BigIntTest, Multiplication) {
    BigInt a(123, 10);
    BigInt b(456, 10);
    EXPECT_EQ(a * b, BigInt(123 * 456));

    BigInt c("-123", 10);
    BigInt d(456, 10);
    EXPECT_EQ(c * d, BigInt(-123 * 456));

    BigInt zero(0, 10);
    EXPECT_EQ(a * zero, zero);
}

TEST(BigIntTest, Division) {
    BigInt a(456, 10);
    BigInt b(123, 10);
    EXPECT_EQ(a / b, BigInt(3));

    BigInt c(123, 10);
    EXPECT_EQ(c / BigInt(1), c);

    EXPECT_THROW(a / BigInt(0), std::invalid_argument);
}

TEST(BigIntTest, Comparison) {
    BigInt a(123, 10);
    BigInt b(456, 10);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(a == b);

    BigInt c("-123", 10);
    BigInt d(123, 10);
    EXPECT_TRUE(c < d);
}

TEST(BigIntTest, IncrementDecrement) {
    BigInt a(123, 10);
    EXPECT_EQ(++a, BigInt(124));
    EXPECT_EQ(a++, BigInt(124));
    EXPECT_EQ(a, BigInt(125));

    BigInt b(123, 10);
    EXPECT_EQ(--b, BigInt(122));
    EXPECT_EQ(b--, BigInt(122));
    EXPECT_EQ(b, BigInt(121));
}

TEST(BigIntTest, IOStream) {
    BigInt num(-12345, 10);
    std::stringstream ss;
    ss << num;
    EXPECT_EQ(ss.str(), "-12345");

    BigInt numRead;
    ss >> numRead;
    EXPECT_EQ(numRead, num);
}