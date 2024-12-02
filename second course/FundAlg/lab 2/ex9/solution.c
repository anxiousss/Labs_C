#include "solution.h"

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int x = 3; x * x <= n; x += 2) {
        if (n % x == 0)
            return 0;
    }
    return 1;
}

void fractional_part_to_int(double x, int* numerator, int* denominator) {
    *numerator = 1;
    *denominator = (int)round(1.0 / (x - floor(x)));

    double eps = 0.00001;
    double approximation = (double)*numerator / *denominator;
    double difference = approximation - x;

    while (fabs(difference) >= eps) {
        if (difference > 0) {
            int diffDenominator = (int)round(1.0 / difference);
            int newNumerator = *numerator * diffDenominator - *denominator;
            int newDenominator = *denominator * diffDenominator;
            *numerator = newNumerator;
            *denominator = newDenominator;
        } else {
            int diffDenominator = (int)round(1.0 / -difference);
            int newNumerator = *numerator * diffDenominator + *denominator;
            int newDenominator = *denominator * diffDenominator;
            *numerator = newNumerator;
            *denominator = newDenominator;
        }

        approximation = (double)*numerator / *denominator;
        difference = approximation - x;
    }

    int divisor = gcd(*numerator, *denominator);
    *numerator /= divisor;
    *denominator /= divisor;
}

void prime_factors(int* factors, int number, int* index) {
    *index = 0;
    if (number <= 1)
        return;
    while (number % 2 == 0) {
        factors[(*index)++] = 2;
        number /= 2;
    }
    for (int x = 3; x * x <= number; x += 2) {
        while (number % x == 0) {
            if (is_prime(x)) {
                factors[(*index)++] = x;
                number /= x;
            }
        }
    }
    if (number > 2) {
        factors[(*index)++] = number;
    }
}

int is_finite(char*** results, int base, int n, ...) {
    if (base <= 1)
        return Invalid_input;
    va_list numbers;
    va_start(numbers, n);
    *results = (char**)malloc(n * sizeof(char*));
    if (!*results) {
        va_end(numbers);
        return Memory_leak;
    }
    int factors_base[1024];
    int base_index = 0;
    prime_factors(factors_base, base, &base_index);
    for (int i = 0; i < n; ++i) {
        double number = va_arg(numbers, double);
        int numerator, denominator;
        fractional_part_to_int(number - floor(number), &numerator, &denominator);
        int factors_denominator[1024];
        int denom_index = 0;
        prime_factors(factors_denominator, denominator, &denom_index);
        int flag = 0;
        for (int j = 0; j < denom_index; ++j) {
            int factor = factors_denominator[j];
            int is_divisor = 0;
            for (int k = 0; k < base_index; ++k) {
                if (factors_base[k] == factor) {
                    is_divisor = 1;
                    break;
                }
            }
            if (!is_divisor) {
                flag = 1;
                break;
            }
        }
        char* msg = malloc(256);
        if (!msg) {
                for (int j = 0; j < i; ++j)
                free((*results)[j]);
            free(*results);
            va_end(numbers);
            return Memory_leak;
        }
        if (flag) {
            sprintf(msg, "Fraction %lf does not have a finite representation in base %d.\n", number, base);
        } else {
            sprintf(msg, "Fraction %lf has a finite representation in base %d.\n", number, base);
        }
        (*results)[i] = msg;
    }
    va_end(numbers);
    return 0;
}