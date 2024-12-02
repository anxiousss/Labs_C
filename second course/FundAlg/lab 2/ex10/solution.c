#include "solution.h"


int expansion_series(double eps, double a, double **result, int n, ...) {
    *result = (double *)malloc((n + 1) * sizeof(double));
    if (*result == NULL)
        return Memory_leak;

    double *coefficients = (double *)malloc((n + 1) * sizeof(double));
    if (coefficients == NULL) {
        free(*result);
        return Memory_leak;
    }

    va_list args;
    va_start(args, n);
    for (int i = 0; i <= n; i++)
        coefficients[i] = va_arg(args, double);
    va_end(args);

    // Precompute a^k for k from 0 to n
    double *a_powers = (double *)malloc((n + 1) * sizeof(double));
    if (a_powers == NULL) {
        free(coefficients);
        free(*result);
        return Memory_leak;
    }
    a_powers[0] = 1.0;
    for (int k = 1; k <= n; k++)
        a_powers[k] = a_powers[k - 1] * a;

    double* i_factorial = (double *)malloc((n + 1) * sizeof(double));
    if (i_factorial == NULL) {
        free(a_powers);
        free(coefficients);
        free(*result);
        return Memory_leak;
    }

    i_factorial[0] = 1.0;
    for (int i = 1; i <= n; i++)
        i_factorial[i] = i_factorial[i - 1] * i;


    for (int i = 0; i <= n; i++) {
        double derivative = 0.0;
        double combination = 1.0;
        double a_power = 1.0;
        for (int j = i; j <= n; j++) {
            if (j > i) {
                combination = combination * (j) / (j - i);
                a_power = a_power * a;
            }
            double term = coefficients[j] * combination * a_power;
            derivative += term;
            if (fabs(term) < eps)
                break;
        }
        (*result)[i] = derivative / i_factorial[i];
    }

    free(i_factorial);
    free(a_powers);
    free(coefficients);
    return 0;
}