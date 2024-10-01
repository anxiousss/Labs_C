#include <stdio.h>
#include <math.h>

typedef enum Errors {
    Invalid_epsilon = 2,
    Invalid_number
};

long double my_atof(const char* str) {
    long double result = 0.0;
    long double fraction = 1.0;
    int sign = 1;
    int i = 0;

    if (str[i] == '-') {
        sign = -1;
        ++i;
    }

    while (str[i] != '.' && str[i] != '\0') {
        result = result * 10.0 + (str[i] - '0');
        ++i;
    }

    if (str[i] == '.') {
        ++i;
    }

    while (str[i] != '\0') {
        fraction /= 10.0;
        result += (str[i] - '0') * fraction;
        ++i;
    }

    return sign * result;
}

int GetOpts(int argc, char** argv, long double* eps, int* x) {
    if (argc != 3) {
        return 1;
    }
    *eps = my_atof(argv[1]);
    const char* procceding_option = argv[2];
    int sign = 1;
    for (int j = 0; procceding_option[j]; ++j) {
        if (procceding_option[j] == '-' && sign == 1)
            sign = -1;
        else if (procceding_option[j] >= '0' && procceding_option[j] <= '9') {
            *x *= 10;
            *x = procceding_option[j] - '0';
        } else {
            return Invalid_number;
        }
    }
    *x *= sign;
    if (eps < 0) {
        return Invalid_epsilon;
    }
    return 0;
}

long double series_a(long double eps, int x) {
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

long double series_b(long double eps, int x) {
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


long double series_c(long double eps, int x) {
    if (x == 1) {
        printf("series doesn't converge\n");
        return 1;
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
    if (x < 1) {
        printf("series doesn't converge\n");
        return 1;
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
    long double eps = 0.0;
    int x = 0;
    if (GetOpts(argc, argv,  &eps, &x)) {
        printf("%s\n", "Incorrect option");
        return 1;
    }
    printf("%Lf\n", series_a(eps, x));
    printf("%Lf\n", series_b(eps, x));
    printf("%Lf\n", series_c(eps, x));
    printf("%Lf\n", series_d(eps, x));
    return 0;
}
