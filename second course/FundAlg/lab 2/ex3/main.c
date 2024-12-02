#include "solution.h"

int main() {
    int total_results;
    char** results = FindFiles("\tq\n\t", 2, &total_results, "1.txt", "2.txt");

    if (!results) {
        printf("Memory allocation failed\n");
        return Memory_leak;
    }
    for (int i = 0; i < total_results; i++) {
        printf("%s", results[i]);
        free(results[i]);
    }
    free(results);
    return 0;
}