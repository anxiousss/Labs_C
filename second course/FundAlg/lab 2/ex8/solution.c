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
        int copy_len = len(number + i) + 1;
        memmove(result + 1, number + i, copy_len);
    } else {
        int copy_len = len(number + i) + 1;
        memmove(result, number + i, copy_len);
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
    c = toupper(c);
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    } else {
        return Invalid_input;
    }
}

int addition(int base, char* a, char* b, char* result) {
    char* a_no_zeros = strdup(a);
    if (!a_no_zeros) {
        return Memory_leak;
    }

    char* b_no_zeros = strdup(b);
    if (!b_no_zeros) {
        free(a_no_zeros);
        return Memory_leak;
    }

    remove_leading_zeros(a_no_zeros, a_no_zeros);
    remove_leading_zeros(b_no_zeros, b_no_zeros);

    int len_a = len(a_no_zeros);
    int len_b = len(b_no_zeros);
    int max_len = len_a > len_b ? len_a : len_b;
    int carry = 0;

    char* temp_result = (char*)malloc((max_len + 2) * sizeof(char));
    if (!temp_result) {
        free(a_no_zeros);
        free(b_no_zeros);
        return Memory_leak;
    }

    int i = 0;
    while (i < len_a || i < len_b || carry > 0) {
        int digit_a = (i < len_a) ? get_digit(a_no_zeros[len_a - 1 - i]) : 0;
        int digit_b = (i < len_b) ? get_digit(b_no_zeros[len_b - 1 - i]) : 0;
        int sum_digit = digit_a + digit_b + carry;
        carry = sum_digit / base;
        temp_result[i] = get_char(sum_digit % base);
        i++;
    }
    temp_result[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = temp_result[j];
        temp_result[j] = temp_result[i - 1 - j];
        temp_result[i - 1 - j] = temp;
    }

    remove_leading_zeros(temp_result, temp_result);
    strcpy(result, temp_result);

    free(a_no_zeros);
    free(b_no_zeros);
    free(temp_result);
    return 0;
}

int sum(int base, int n, char** result, ...) {
    va_list numbers;
    va_start(numbers, result);

    char* num = va_arg(numbers, char*);
    char* temp_sum = strdup(num);
    if (!temp_sum) {
        va_end(numbers);
        return Memory_leak;
    }

    for (int i = 1; i < n; i++) {
        char* next_num = va_arg(numbers, char*);
        char* new_sum = (char*)malloc((len(temp_sum) + len(next_num) + 2) * sizeof(char));
        if (!new_sum) {
            free(temp_sum);
            va_end(numbers);
            return Memory_leak;
        }
        addition(base, temp_sum, next_num, new_sum);
        free(temp_sum);
        temp_sum = new_sum;
    }

    va_end(numbers);

    if (*result) {
        free(*result);
    }
    *result = temp_sum;
    return 0;
}
