#include "solution.h"

int main() {
    char* result = malloc(sizeof(char ) * 100);
    sum(16, 3, &result, "1A3", "2B4", "3C5");
    printf("%s", result);
    return 0;
}