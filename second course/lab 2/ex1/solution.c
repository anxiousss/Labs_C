
#include "solution.h"



double geometric_mean(int n, ...) {
    if (n <= 0) {
        return 0.0;
    }

    va_list args;
    va_start(args, n);

    double product = 1.0;
    for (int i = 0; i < n; ++i) {
        double num = va_arg(args, double);
        product *= num;
    }

    va_end(args);

    return pow(product, 1.0 / n);
}


double recurse_exp(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    }

    if (exponent > 0) {
        if (exponent % 2 == 0) {
            double temp = recurse_exp(base, exponent / 2);
            return temp * temp;
        }
        else {
            return base * recurse_exp(base, exponent - 1);
        }
    }

    if (exponent < 0) {
        return 1.0 / recurse_exp(base, -exponent);
    }
    return 0;
}