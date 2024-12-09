#include "solution.h"

double pseudo_scalar_product(Coord a, Coord b, Coord c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int is_convex(int n, ...) {
    va_list vertexes;
    va_start(vertexes, n);
    Coord coords[n];
    if (n <= 2) return 1;
    for (int i = 0; i < n; ++i) {
        coords[i] = va_arg(vertexes, Coord);
    }
    va_end(vertexes);

    int sign;
    for (int i = 0; i < n; ++i) {
        double product = pseudo_scalar_product(coords[i], coords[(i + 1) % n], coords[(i + 2) % n]);
        sign = (product > 0) ? 1 : -1;
        if (product * sign <= 0) {
            return 0;
        }
    }
    return 1;
}

double polynomial(double x, int n, ...) {
    va_list coefficients;
    va_start(coefficients, n);
    double value = 0, coefficient;
    for (int i = 0; i < n; ++i) {
        coefficient = va_arg(coefficients, double );
        value = value * x + coefficient;
    }
    va_end(coefficients);
    return value;
}

int is_valid_number(int base, char* number) {
    for (int i = 0; number[i] != '\0'; i++) {
        if (isdigit(number[i])) {
            if (number[i] - '0' >= base) {
                return 0;
            }
        } else if (isalpha(number[i])) {
            if (toupper(number[i]) - 'A' + 10 >= base) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    return 1;
}


int is_kaprekar(int base, char* number) {
    if (!is_valid_number(base, number)) {
        return Invalid_input;
    }
    long long num = 0;
    int len = strlen(number);
    for (int i = 0; i < len; i++) {
        num = num * base + (number[i] - (number[i] >= 'A' ? 'A' - 10 : '0'));
    }

    if (num == 1)
        return 1;

    long long square = num * num;

    char square_str[100];
    int idx = 0;
    while (square > 0) {
        int digit = square % base;
        square_str[idx++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
        square /= base;
    }
    square_str[idx] = '\0';

    for (int i = 0; i < idx / 2; i++) {
        char temp = square_str[i];
        square_str[i] = square_str[idx - 1 - i];
        square_str[idx - 1 - i] = temp;
    }

    int square_len = strlen(square_str);
    for (int i = 1; i < square_len; i++) {
        char left_part[100], right_part[100];
        strncpy(left_part, square_str, i);
        left_part[i] = '\0';
        strcpy(right_part, square_str + i);

        long long left = 0, right = 0;
        for (int j = 0; j < i; j++) {
            left = left * base + (left_part[j] - (left_part[j] >= 'A' ? 'A' - 10 : '0'));
        }
        for (int j = 0; j < square_len - i; j++) {
            right = right * base + (right_part[j] - (right_part[j] >= 'A' ? 'A' - 10 : '0'));
        }

        if (right > 0 && left + right == num) {
            return 1;
        }
    }

    return 0;
}

int kaprekar_handler(int base, char*** kaprekar_numbers, int n, ...) {
    if (n < 0 || base < 2 || base > 36)
        return Invalid_input;

    va_list args;
    va_start(args, n);

    char** numbers = (char**)(malloc(n * sizeof(char*)));
    if (!numbers)
        return Memory_leak;

    for (int i = 0; i < n; ++i) {
        numbers[i] = (char*)(malloc(sizeof(char ) * 1000));
        if (!numbers[i]) {
            for (int j = 0; j < i; ++j) {
                free(numbers[i]);
            }
            free(numbers);
            return Memory_leak;
        }
    }


    for (int i = 0; i < n; i++) {
        char* number = va_arg(args, char*);
        int res = is_kaprekar(base, number);
        if (res == 2) {
            for (int j = 0; j < n; ++j) {
                free(numbers[j]);
            }
            free(numbers);
            return Invalid_input;
        } else if (res == 1) {
            sprintf(numbers[i], "%s is a Kaprekar number in base %d.\n", number, base);
        } else {
            sprintf(numbers[i], "%s is not a Kaprekar number in base %d.\n", number, base);
        }
    }

    va_end(args);

    *kaprekar_numbers = numbers;
    return 0;
}

