#include "solution.h"

int main() {
    int base;
    scanf("%d", &base);
    if (2 > base || base > 36) {
        printf("Invalid input");
        return Invalid_input;
    }
    char string[256];
    int max = INT_MIN;
    char max_string[256];
    while (1) {
        scanf("%s", string);
        if (strcmp(string, "Stop") == 0) {
            break;
        }
        char result[256];
        int err = convert(base, 10, string, result);
        if (err) {
            printf("Invalid input");
            return err;
        }
        if (max < abs(atoi(result))) {
            max = abs(atoi(result));
            strcpy(max_string, result);
        }
    }
    int bases[4]= {9, 18, 27, 36};
    for (int i = 0; i < 4; ++i) {
        char tmp[256];
        int err = convert(10, bases[i], max_string, tmp);
        if (err) {
            printf("Invalid input");
            return err;
        }
        printf("Max number with base %d: %s\n", bases[i], tmp);
    }
}
