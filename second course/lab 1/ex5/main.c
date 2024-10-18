#include <stdio.h>
#include <math.h>

typedef enum Errors {
    Invalid_input = 1,
    Not_converge = 2
} Errors;

int my_atof(const char *string, long double *result) {
    int sign = 1;
    long double number = 0.0;
    long double fraction = 0.0;
    int has_fraction = 0;
    int fraction_digits = 0;

    for (int j = 0; string[j] != '\0'; ++j) {
        char c = string[j];

        if (c == '-' && j == 0) {
            sign = -1;
        } else if (c == '.' && !has_fraction) {
            has_fraction = 1;
        } else if (c >= '0' && c <= '9') {
            if (has_fraction) {
                fraction = fraction * 10.0 + (c - '0');
                fraction_digits++;
            } else {
                number = number * 10.0 + (c - '0');
            }
        } else {
            return Invalid_input;
        }
    }

    while (fraction_digits > 0) {
        fraction /= 10.0;
        fraction_digits--;
    }

    *result = sign * (number + fraction);
    return 0;
}



int GetOpts(int argc, char** argv, long double* eps, long double* x) {
    if (argc != 3) {
        return 1;
    }
    my_atof(argv[1], eps);
    my_atof(argv[2], x);
    if (eps < 0) {
        return Invalid_input;
    }
    return 0;
}

long double series_a(long double eps, long double x) {
    long double prev = -1.0, current = 1.0, total = 1;
    int n = 1;
    while (fabsl(prev - total) > eps) {
        prev = total;
        current *= x;
        current /= n;
        ++n;
        total += current;
    }
    return total;
}

long double series_b(long double eps, long double x) {
    long double prev = -1.0, current = 1.0, total = 1;
    int n = 1;
    while (fabsl(prev - total) > eps) {
        prev = total;
        current *= (-1  * x * x);
        current /= ((2 * n) * (2 * n - 1 ));
        ++n;
        total += current;
    }
    return total;
}


long double series_c(long double eps, long double x) {
    if (x >= 1) {
        return Not_converge;
    }
    long double prev = -1.0, current = 1.0, total = 1;
    int n = 1;
    while (fabsl(prev - total) > eps) {
        prev = total;
        current *= (9 * x * x * n * n);
        current /= ((3 * n - 1) * (3 * n - 2));
        ++n;
        total += current;
    }
    return total;
}

long double series_d(long double eps, long double x) {
    if (x >= 1) {
        return Not_converge;
    }
    long double prev = -1.0, current = 1.0, total = 0;
    int n = 1;
    while (fabsl(prev - total) > eps) {
        prev = total;
        current *= (-1 * (2 * n - 1) * x * x);
        current /= (2 * n);
        ++n;
        total += current;
    }
    return total;
}

int main(int argc, char** argv) {
    long double eps = 0.0, x = 0.0;
    int result = GetOpts(argc, argv, &eps, &x);
    if (result != 0) {
        return result;
    }
    long double (*series[4])(long double, long double) = {
            series_a,
            series_b,
            series_c,
            series_d
    };

    for (int i = 0; i < 4; ++i) {
        long double sum = series[i](eps, x);
        if (sum == 2) {
            printf("Not converge\n");
        } else {
            printf("%Lf\n", sum );
        }
    }
    return 0;
}
