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

long long int factorial(int number) {

    long long int result = 1;
    for (int i = 1; i <= number; i++) {
        result *= i;
    }
    if (result < 0) {
        return Overflow;
    }
    return result;
}

long double combinations(int n, int k) {
    return (long double)factorial(n) / (long double)(factorial(k) * factorial(n - k));
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
    long double prev;
    long double cur = 4;
    long double diff = 1;
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
    return cosl(x) + 1;
}

long double equation_pi_deriative1(long double x) {
    return -sinl(x);
}

long double equation_pi_deriative2(long double x) {
    return -cosl(x);
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

long double limit_gamma(int m) {
    if (m == 1)
        return 0.0;

    int sign = -1;
    long double comb = combinations(m, 2), l = logl(2);
    long double sum = comb * sign * l;
    for (int k = 3; k < m + 1; ++k) {
        if (m < k)
            return sum;

        if (comb < 0)
            return Overflow;

        sum += comb * (sign / (long double)k) * l;
        l += logl(k);
        if (m == k) {
            comb = 1;
        }   else {
            comb *= (m - k);
            comb /= (k + 1);
        }
        printf("combinations: %Lf log: %Lf sum: %Lf\n", comb, l, sum);
        sign *= -1;
    }
    return sum;
}

long double series_gamma(long double part, int n) {
    long double value = 1.0/(powl(roundl(sqrtl(n)), 2)) - 1.0/n;
    if (part == value) {
        return Equal;
    }
    return 1.0/(powl(floorl(sqrtl(n)), 2)) - 1.0/n;
}
long double compute_limit_sqrt2(long double eps) {
    long double diff = 1;
    long double res = -0.5;
    long double prev;
    while (diff > eps) {
        prev = res;
        res = limit_sqrt2(prev);
        diff = fabsl(prev - res);
    }
    return res;
}

long double compute_series_sqrt2(long double eps) {
    int n = 1;
    long double prod = 1;
    long double prev;
    long double cur;
    long double diff = 1;
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
    long double cur = func(n);
    long double diff = 1;
    long double prev ;
    while (diff > eps) {
        prev = cur;
        n *= 2;
        cur = func(n);
        if (cur == Overflow) {
            return prev;
        }
        diff = fabsl(prev - cur);
    }
    return cur;
}



long double series_sum(long double eps,  long double(*func)(long double, int), int multiplier, int n) {
    long double cur = 0.0;
    long double prev;
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

int newton_method(long double eps, long double a, long double b, long double* res, long double(*f)(long double), long double(*fd1)(long double), long double(*fd2)(long double)) {
    long double x = (a + b) / 2;
    for (long double i = a; i <= b ; i += eps) {
        if (f(i) * fd2(i) >= fd1(i) * fd1(i)) {
            return 1;
        }
    }
    while (fabsl(f(x)) >= eps) {
        x = x - (f(x) / fd1(x));
    }
    *res = x;
    return 0;
}
