#include "../include/big_int.hpp"

int main() {
    BigInt a(123, 10);
    BigInt b(-126, 10);

    std::cout << a + b << std::endl;
}