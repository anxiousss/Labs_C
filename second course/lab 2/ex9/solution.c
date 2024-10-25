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
    for (int x = 3; x <= sqrt(n); x += 2) {
        if (n % x == 0)
            return 0;
    }
    return 1;
}

int fractional_part_to_int(double number, int* numerator, int* denominator) {
    if (number < 0 || number >= 1)
        return Invalid_input;
    *numerator = 1;
    *denominator = (int)round(1.0 / (number - (int)number));

    double eps = 0.00001;
    double approximation = (double)*numerator / *denominator;
    double difference = approximation - number;

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
        difference = approximation - number;
    }
    int divisor = gcd(*numerator, *denominator);
    *numerator /= divisor;
    *denominator /= divisor;
    return 0;
}

void prime_factors(int* factors, int number, int* index) {
    int i = 0;
    if (number % 2 == 0) {
        factors[i] = 2;
        ++i;
        while (number % 2 == 0)
            number /= 2;
    }

    for (int x = 3; x <= sqrt(number); x += 2) {
        if (number % x == 0 && is_prime(x)) {
            factors[i] = x;
            ++i;
            while (number % x == 0)
                number /= x;
        }
    }
    if (number > 2)
        factors[i] = number;
    *index = i;
}

int is_finite(char*** results, int base, int n, ...) {
    va_list numbers;
    va_start(numbers, n);

    int factors_base[1024];
    int base_index = 0;
    prime_factors(factors_base, base, &base_index);

    *results = (char**)malloc(n * sizeof(char*));

    for (int i = 0; i < n; ++i) {
        double number = va_arg(numbers, double);
        int index = 0, flag = 0, denominator, numerator;
        int factors[1024];
        int err = fractional_part_to_int(number, &numerator, &denominator);
        if (err)
            return err;
        prime_factors(factors, denominator, &index);

        for (int j = 0; j < index; ++j) {
            int is_divisor = 0;
            for (int k = 0; k < base_index; ++k) {
                if (factors[j] == factors_base[k] && factors[j] != 0 && factors_base[k] != 0) {
                    is_divisor = 1;
                    break;
                }
            }
            if (!is_divisor) {
                flag = 1;
                break;
            }
        }

        char buffer[128];
        buffer[0] = '\0';
        const char* s = flag ? "NO" : "YES";
        sprintf(buffer, "Number %f Base %d finite %s", number, base, s);
        (*results)[i] = strdup(buffer);
    }
    va_end(numbers);
    return 0;
}