#include "solution.h"

int main() {
    char** results = (char**)(malloc(sizeof(char*) * 10));
    if (!results) {
        printf("Memory leak");
        return Memory_leak;
    }
    for (int i = 0; i < 10; ++i) {
        results[i] = (char*)(malloc(sizeof(char ) * 128));
        if (!results[i]) {
            for (int j = 0; j < i; ++j) {
                free(results[j]);
            }
            return Memory_leak;
        }
    }
    int n = 1, base = 8;
    int err = is_finite(&results, base, n, -1.893);
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
