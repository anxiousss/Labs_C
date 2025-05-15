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

    EXPECT_THROW(BigInt("123", 15), std::invalid_argument);

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
TEST(BigIntTest, AssignmentOperator) {
    BigInt a(123, 10);
    BigInt b;

    b = a;
    EXPECT_EQ(b, BigInt(123));
    EXPECT_EQ(b.getBase(), 10);

}


TEST(BigIntTest, CompoundOperators) {
    BigInt a(100, 10);
    BigInt b(3, 10);

    a *= b;
    EXPECT_EQ(a, BigInt(300));
    EXPECT_EQ(a.getBase(), 10);

    a /= BigInt(2, 10);
    EXPECT_EQ(a, BigInt(150));
}

TEST(BigIntDiffBaseTest, CorrectBaseInterpretation) {
    BigInt a("10000", 100);
    EXPECT_EQ(a, BigInt(10000, 10));

    BigInt b("10203", 100);
    EXPECT_EQ(b, BigInt(10203, 10));
}

TEST(BigIntDiffBaseTest, FixedMixedBaseOperations) {

    BigInt a("1000", 100);
    BigInt b(10000, 10);
    EXPECT_EQ(a + b, BigInt(11000, 10));

    BigInt c("25000", 100);
    BigInt d(0, 10);
    EXPECT_EQ(c - d, BigInt(25000, 10));

    BigInt e("200", 100);
    BigInt f(3, 10);
    EXPECT_EQ(e * f, BigInt(600, 10));

    BigInt g("10000", 1000);
    BigInt h(1000, 10);
    EXPECT_EQ(g / h, BigInt(10, 10));
}

TEST(BigIntLargeNumbersTest, LargeAddition) {
    BigInt a("123456789012345678901234567890", 10);
    BigInt b("987654321098765432109876543210", 10);
    BigInt expected("1111111110111111111011111111100", 10);
    EXPECT_EQ(a + b, expected);
}

TEST(BigIntLargeNumbersTest, LargeMultiplication) {
    BigInt a("123456789012345678901234567890", 10);
    BigInt b("2", 10);
    BigInt expected("246913578024691357802469135780", 10);
    EXPECT_EQ(a * b, expected);
}

TEST(BigIntLargeNumbersTest, LargeDivision) {
    BigInt a("246913578024691357802469135780", 10);
    BigInt b("2", 10);
    BigInt expected("123456789012345678901234567890", 10);
    EXPECT_EQ(a / b, expected);
}


TEST(BigIntLargeBaseTest, Base10000Operations) {
    BigInt a("5000", 10000);
    BigInt b("3", 10000);
    BigInt expected("15000", 10000);
    EXPECT_EQ(a * b, expected);

    BigInt c("9999", 10000);
    BigInt d("1", 10000);
    BigInt sum("10000", 10000);
    EXPECT_EQ(c + d, sum);
}

TEST(BigIntMixedBaseTest, CrossBaseOperations) {
    BigInt a("1000000", 10);
    BigInt b("1000", 1000);
    BigInt expected("1001000", 10);
    EXPECT_EQ(a + b, expected);
}

TEST(BigIntEdgeCaseTest, ExtremelyLargeNumber) {
    std::string large_number(1000, '9');
    BigInt a(large_number, 10);
    BigInt b("1", 10);
    BigInt result = a + b;

    std::string expected(large_number.size() + 1, '0');
    expected[0] = '1';
    EXPECT_EQ(result, BigInt(expected, 10));
}