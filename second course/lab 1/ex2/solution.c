

#include "solution.h"


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

long double limit_e(int n) {
    return powl(1.0 + 1.0 / n, (long double) n);
}

long double series_e(long double part, int n) {
    if (n == 0) return part;
    return part * 1.0 / (long double )n ;
}

long double equation_e(long double x) {
    return logl(x) - 1;
}

long double compute_limit_pi(long double eps) {
    int n = 2;
    long double prev, cur = 4, diff = 1;
    while (diff > eps) {
        prev = cur;
        cur *= (4.0 * n * (n - 1)) / powl(2 * n - 1, 2.0);
        diff = fabsl(prev - cur);
        ++n;
    }
    return cur;
}


long double series_pi(long double part, int n) {
    if (n == 0) return -1;
    return powl(-1.0, n - 1.0) / (2 * (long double)n - 1);
}

long double equation_pi(long double x) {
    return cosl(x / 2.0);
}
long double limit_ln2(int n) {
    return (long double)n * (powl(2, 1.0 / (long double)n) - 1);
}

long double series_ln2(long double part, int n) {
    if (n == 0) return -1;
    return powl(-1.0, (long double)n - 1.0) / (long double)n;
}

long double equation_ln2(long double x) {
    return expl(x) - 2;
}

long double limit_sqrt2(long double prev) {
    return prev - (prev * prev) / 2.0 + 1;
}

long double series_sqrt2(int n) {
    if (n < 2) return Invalid_input;
    return exp2l(exp2l(-n));
}

long double equation_sqrt2(long double x) {
    return powl(x, 2.0) - 2;
}

long double compute_limit_sqrt2(long double eps) {
    long double diff = 1, res = -0.5, prev;
    while (diff > eps) {
        prev = res;
        res = limit_sqrt2(prev);
        diff = fabsl(prev - res);
    }
    return res;
}

long double compute_series_sqrt2(long double eps) {
    int n = 1;
    long double prod = 1, prev, cur, diff = 1;
    while (diff > eps) {
        prev = prod;
        cur = series_sqrt2(++n);
        prod *= cur;
        diff = fabsl(prev - prod);
    }
    return prod;
}

long double calculate_limit(long double eps, long double (*func)(int n)) {
    int n = 1;
    long double cur = func(n), diff = 1, prev;
    while (diff > eps) {
        prev = cur;
        n *= 2;
        cur = func(n);
        diff = fabsl(prev - cur);
    }
    return cur;
}



long double series_sum(long double eps,  long double(*func)(long double, int), int multiplier, int n) {
    long double cur = 0.0, prev = 0.0;
    long double value = func(1.0, n);
    long double diff = 1.0;

    while (diff > eps) {
        prev = cur;
        value = func(value, n++);
        cur += value;
        diff = fabsl(prev - cur);
    }

    return multiplier * cur;
}

int dichotomy_method(long double eps, long double(*func)(long double ), long double a, long double b, long double* res) {
    if (func(a) * func(b) >= 0) {
        printf("Wrong interval");
        return 1;
    }

    while (fabsl(a - b) > eps) {
        if (func(a) * func((a + b) / 2) > 0) {
            a = (a + b) / 2;
        }
        else {
            b = (a + b) / 2;
        }
    }

    *res = (a + b) / 2.0;
    return 0;
}

long double iteration_method(long double eps, long double a, long double b, long double(*func)(long double)) {
    long double x0 = (a + b) / 2, x_current = 0;
    int iterations = 100, i = 0;
    while (i < iterations) {
        x_current = func(x0);
        if (fabsl(x_current - x0) < eps) {
            return x_current;
        }
        x0 = x_current;
        ++i;
    }
    return x_current;
}