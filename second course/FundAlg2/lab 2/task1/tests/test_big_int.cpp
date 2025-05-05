#include "../include/big_int.hpp"

int main() {
    BigInt b("123", 10);
    BigInt a("123", 10);
    std::cout << a + b;
}