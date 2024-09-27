#include <stdio.h>
#include <math.h>

double my_atof(const char* str) {
    double result = 0.0;
    double fraction = 1.0;
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


int GetOpts(int argc, char** argv, double* eps) {
    if (argc != 2) {
        return 1;
    }
    *eps = my_atof(argv[1]);
    if (eps < 0) {
        printf("invalid epsilon");
        return 2;
    }
    return 0;
}

double integral(double (*func)(double), double a, double b, double eps) {
    double prev = -1.0, current = 0.0;
    int n = 10;
    double step;

    while (fabs(current - prev) > eps) {
        prev = current;
        current = 0.0;
        step = (b - a) / n;
        for (int i = 0; i < n; ++i) {
            double x = a + i * step + step / 2.0;
            current += func(x) * step;
        }
        n *= 2;
    }
    return current;
}

double f1(double x) {
    return log(1 + x) / x;
}

double f2(double x) {
    return exp(-pow(x, 2) / 2);
}

double f3(double x) {
    return log(1/(1 - x));
}

double f4(double x) {
    return pow(x, x);
}

int main(int argc, char** argv) {
    double eps = 0.0;
    if (GetOpts(argc, argv,  &eps)) {
        printf("%s\n", "Incorrect option");
        return 1;
    }
    printf("%f\n", integral(f1, 0, 1 ,eps));
    printf("%f\n", integral(f2, 0, 1 ,eps));
    printf("%f\n", integral(f3, 0, 1 ,eps));
    printf("%f\n", integral(f4, 0, 1 ,eps));
    return 0;
}
