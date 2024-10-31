#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int convert(int old_base, int new_base, char* string, char* result) {


    int is_negative = 0;
    if (string[0] == '-') {
        is_negative = 1;
        string++;
    }

    long long decimal = 0;
    int length = len(string);
    for (int i = 0; i < length; i++) {
        int digit;
        if (string[i] >= '0' && string[i] <= '9') {
            digit = string[i] - '0';
        } else if (string[i] >= 'A' && string[i] <= 'Z') {
            digit = string[i] - 'A' + 10;
        } else if (string[i] >= 'a' && string[i] <= 'z') {
            digit = string[i] - 'a' + 10;
        } else {
            return Invalid_input;
        }
        if (digit < 0 || digit >= old_base) {
            return Invalid_input;
        }
        decimal = decimal * old_base + digit;
    }

    int index = 0;
    while (decimal > 0) {
        int digit = decimal % new_base;
        if (digit >= 0 && digit <= 9) {
            digit += '0';
        } else if (digit >= 10 && digit <= 35) {
            digit = digit - 10 + 'A';
        } else {
            digit = '\0';
        }
        result[index++] = digit;
        decimal /= new_base;
    }
    if (index == 0) {
        result[index++] = '0';
    }
    if (is_negative) {
        result[index++] = '-';
    }

    result[index] = '\0';

    for (int i = 0; i < index / 2; i++) {
        char temp = result[i];
        result[i] = result[index - i - 1];
        result[index - i - 1] = temp;
    }

    return 0;
}

