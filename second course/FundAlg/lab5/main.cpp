#include "solution.h"


int main() {
    LogicalValuesArray lva(0x169);

    std::cout << lva.sheffer_stroke(LogicalValuesArray(0x372126)) << std::endl;

    std::cout << lva.get_bit(1) << std::endl;

    std::cout << lva << std::endl;

    return 0;
}