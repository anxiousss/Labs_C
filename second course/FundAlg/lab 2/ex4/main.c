#include "solution.h"


int main() {
    char** kaprekar_numbers;
    int base = 12, n = 4;
    int res = kaprekar_handler(base, &kaprekar_numbers, n, "1", "BB", "56", "66");
    switch (res) {
        case Invalid_input:
            printf("Invalid input");
            return Invalid_input;
        case Memory_leak:
            printf("Memory leak");
            return Memory_leak;
        default:
            break;
    }

    for (int i = 0; i < n; i++) {
        printf("%s", kaprekar_numbers[i]);
        free(kaprekar_numbers[i]);
    }
    free(kaprekar_numbers);

    return 0;
}
