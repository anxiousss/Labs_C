#include "solution.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid input");
        return Invalid_input;
    }
    long double eps = 0.0, res;
    int err = my_atof(argv[1], &eps);
    if (err) {
        printf("Invalid input");
        return err;
    }
    printf("            euler number \n");
    printf("e limit      = %.16Lf\n", calculate_limit(eps, limit_e));
    printf("e series     = %.16Lf\n", series_sum(eps, series_e, 1, 0));
    err = dichotomy_method(eps, equation_e, 1.0, 3.0, &res);
    if (err)
        return err;
    printf("e equation   = %.16Lf\n", res);

    printf(              "Pi number\n");
    printf("pi limit     = %.16Lf\n", compute_limit_pi(eps));
    printf("pi series    = %.16Lf\n", series_sum(eps, series_pi, 4, 1));
    err = dichotomy_method(eps, equation_pi, 1.0, 4.0, &res);
    if (err)
        return err;
    printf("pi equation  = %.16Lf\n", res);

    printf("             ln2\n");
    printf("ln2 limit    = %.16Lf\n", calculate_limit(eps, limit_ln2));
    printf("ln2 series   = %.16Lf\n", series_sum(eps, series_ln2, 1, 1));
    err = dichotomy_method(eps, equation_ln2, 0.0, 1.0, &res);
    if (err)
        return err;
    printf("ln2 equation = %.16Lf\n", res);

    printf("            sqrt2\n");
    printf("sqrt2 limit   = %.16Lf\n", compute_limit_sqrt2(eps));
    printf("sqrt2 series  = %.16Lf\n", compute_series_sqrt2(eps));
    err = dichotomy_method(eps, equation_sqrt2, 1.0, 2.0, &res);
    if (err)
        return err;
    printf("sqr2 equation = %.16Lf\n", res);


    return 0;
}
