#include "solution.h"

int main() {

    ComplexNumber a(1, 2);
    ComplexNumber b(3, 4);
    ComplexNumber c = a / b;

    std::cout << c << std::endl;
    a = b;
    std::cout << a << std::endl;

    return 0;
}