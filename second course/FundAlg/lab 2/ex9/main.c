#include "solution.h"

int main() {
    char** results = NULL;
    int n = 5, base = 8;
    int err = is_finite(&results, base, n, 0.875, 0.125, 0.375, 0.5, 0.04);
    if (err) {
        printf("Invalid input");
        return Invalid_input;
    }
    for (int i = 0; i < n; ++i) {
        printf("%s\n", results[i]);
        free(results[i]);
    }
    free(results);
    return 0;
}