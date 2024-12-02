#include "solution.h"


int main() {
    char* result = (char*)malloc(1000 * sizeof(char));
    if (!result) {
        printf("Memory leak");
        return Memory_leak;
    }

    int err = sum(36, 3, &result, "Z", "ABC4", "0986");
    if (err) {
        printf("Error: %d\n", err);
        free(result);
        return err;
    }
    printf("Sum: %s\n", result);

    free(result);
    return 0;
}