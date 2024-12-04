#include "solution.h"


int main() {
    char* result = (char*)malloc(1000 * sizeof(char));
    if (!result) {
        printf("Memory leak");
        return Memory_leak;
    }

    int err = sum(36, 3, &result, "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ", "ABC4", "0986");

    switch (err) {
        case Invalid_input:
            printf("Invalid input");
            free(result);
            return Invalid_input;
        case Memory_leak:
            free(result);
            printf("Memory leak");
            return Invalid_input;

        default:
            printf("Sum: %s\n", result);
            free(result);
            return 0;
    }
}