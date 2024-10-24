#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

void remove_leading_zeros(const char* number, char* result) {
    int length = len(number);
    int i = 0;
    int is_negative = 0;

    if (number[0] == '-') {
        is_negative = 1;
        i++;
    }
    while (i < length && number[i] == '0') {
        i++;
    }

    if (i == length) {
        if (is_negative) {
            strcpy(result, "-0");
        } else {
            strcpy(result, "0");
        }
        return;
    }

    if (is_negative) {
        result[0] = '-';
        strcpy(result + 1, number + i);
    } else {
        strcpy(result, number + i);
    }
}

char get_char(int digit) {
    if (digit < 10) {
        return '0' + digit;
    } else {
        return 'A' + digit - 10;
    }
}

int get_digit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    } else {
        return Invalid_input;
    }
}

int addition(int base, char* a, char* b, char* result) {
    char* a_no_zeros = (char*)(malloc(sizeof(char) * (len(a) + 2)));
    char* b_no_zeros = (char*)(malloc(sizeof(char) * (len(b) + 2)));
    if (!a_no_zeros) {
        return Memory_leak;
    }

    if (!b_no_zeros) {
        free(a_no_zeros);
        return Memory_leak;
    }

    remove_leading_zeros(a, a_no_zeros);
    remove_leading_zeros(b, b_no_zeros);
    int len_a = len(a_no_zeros);
    int len_b = len(b_no_zeros);
    int max_len = len_a > len_b ? len_a : len_b;
    int carry = 0;

    result[max_len + 1] = '\0';

    for (int i = 0; i < max_len; i++) {
        int digit_a = (i < len_a) ? get_digit(a_no_zeros[len_a - 1 - i]) : 0;
        int digit_b = (i < len_b) ? get_digit(b_no_zeros[len_b - 1 - i]) : 0;
        int sum = digit_a + digit_b + carry;
        carry = sum / base;
        result[max_len - i] = get_char(sum % base);
    }
    free(a_no_zeros);
    free(b_no_zeros);
    if (carry > 0) {
        result[0] = get_char(carry);
    } else {
        memmove(result, result + 1, max_len + 1);
    }
    return 0;
}

int sum(int base, int n, char** result, ...) {
    va_list numbers;
    va_start(numbers, result);
    char* number = va_arg(numbers, char*);
    char* start = (char*)(malloc(sizeof(char) * 100));
    if (!start) {
        return Memory_leak;
    }
    strcpy(start, "0");

    for (int i = 0; i < n; ++i) {
        int err = addition(base, start, number, *result);
        if (err) {
            free(start);
            return err;
        }
        number = va_arg(numbers, char*);
        strcpy(start, *result);
    }
    free(start);
    va_end(numbers);
    return 0;
}