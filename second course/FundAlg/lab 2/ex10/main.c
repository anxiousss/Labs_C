#include "solution.h"

int main() {
    double *result;
    int degree = 2;
    double a = 2.0;
    double eps = 1e-10;

    expansion_series(eps, a, &result, degree, 2.0, 3.0, 4.0);

    for (int i = 0; i <= degree; i++) {
        printf("%f(x - %f)^%d ", result[i], a, i);
        if (i != degree)
            printf("+ ");
    }

    free(result);
    return 0;
}
