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
    BigInt a("100", 100);
    EXPECT_EQ(a, BigInt(10000, 10));

    BigInt b("123", 100);
    EXPECT_EQ(b, BigInt(10203, 10));
}

TEST(BigIntDiffBaseTest, FixedMixedBaseOperations) {

    BigInt a("100", 100);
    BigInt b(1000, 10);
    EXPECT_EQ(a + b, BigInt(11000, 10));

    BigInt c("500", 100);
    BigInt d(25000, 10);
    EXPECT_EQ(c - d, BigInt(25000, 10));

    BigInt e("200", 100);
    BigInt f(3, 10);
    EXPECT_EQ(e * f, BigInt(60000, 10));

    BigInt g("100", 1000);
    BigInt h(1000, 10);
    EXPECT_EQ(g / h, BigInt(1000, 10));
}

TEST(BigIntTest, ModuloOperator) {
    BigInt a(17, 10);
    BigInt b(5, 10);
    EXPECT_EQ(a % b, BigInt(2, 10));

    BigInt c(-17, 10);
    EXPECT_EQ(c % b, BigInt(-2, 10));

    BigInt d(3, 10);
    BigInt e(5, 10);
    EXPECT_EQ(d % e, BigInt(3, 10));

    BigInt f("100", 100);
    BigInt g(999, 10);
    EXPECT_EQ(f % g, BigInt(10000 % 999));

    BigInt h("123456789", 10);
    BigInt i("98765", 10);
    EXPECT_EQ(h % i, BigInt(123456789 % 98765));
}

TEST(BigIntTest, ModuloEdgeCases) {
    BigInt a(12345, 10);
    EXPECT_EQ(a % BigInt(1, 10), BigInt(0, 10));

    EXPECT_EQ(BigInt(0, 10) % BigInt(5, 10), BigInt(0, 10));
}


TEST(BigIntTest, ModExpBasic) {
    EXPECT_EQ(BigInt::mod_exp(BigInt(3), BigInt(4), BigInt(5)), BigInt(1));
    EXPECT_EQ(BigInt::mod_exp(BigInt(2), BigInt(5), BigInt(10)), BigInt(2));
    EXPECT_EQ(BigInt::mod_exp(BigInt(5), BigInt(3), BigInt(13)), BigInt(8));
}

TEST(BigIntTest, ModExpEdgeCases) {
    EXPECT_EQ(BigInt::mod_exp(BigInt(5), BigInt(0), BigInt(7)), BigInt(1));
    EXPECT_EQ(BigInt::mod_exp(BigInt(0), BigInt(0), BigInt(2)), BigInt(1));
    EXPECT_EQ(BigInt::mod_exp(BigInt(7), BigInt(1), BigInt(3)), BigInt(1));
    EXPECT_EQ(BigInt::mod_exp(BigInt(10), BigInt(5), BigInt(1)), BigInt(0));
    EXPECT_EQ(BigInt::mod_exp(BigInt(0), BigInt(5), BigInt(7)), BigInt(0));
}

TEST(BigIntTest, ModExpNegativeBase) {
    EXPECT_EQ(BigInt::mod_exp(BigInt(-2), BigInt(3), BigInt(5)), BigInt(-3));
    EXPECT_EQ(BigInt::mod_exp(BigInt(-3), BigInt(2), BigInt(4)), BigInt(1));
}

TEST(BigIntTest, ModExpModComparison) {
    EXPECT_EQ(BigInt::mod_exp(BigInt(3), BigInt(4), BigInt(10)), BigInt(1));
    EXPECT_EQ(BigInt::mod_exp(BigInt(7), BigInt(2), BigInt(5)), BigInt(4));
}