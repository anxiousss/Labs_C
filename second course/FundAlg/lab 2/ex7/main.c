#include "solution.h"

int main() {
    long double (*equation[10])(long double) = {
            eq1,
            eq2,
            eq3,
            eq4,
            eq5,
            eq6,
            eq7,
            eq8,
            eq9,
            eq10
    };
    long double left_borders[10] = {1.0, 0.0, 0.0, 0.0, 1.0, -2.0, 0.0, 0.0, 0.1, 0.0};
    long double right_borders[10] = {3.0, M_PI, 1.5, 1.0, 3.0, 2.0,  2.0, 2.0, 1.0, M_PI};
    long double epsilons[10] = {0.01, 0.000000000001, 0.00001, 0.001, 0.01, 0.00001, 0.1, 0.00001, 0.1, 0.000001};
    long double res = 0.0;
    int err;
    for (int i = 0; i < 10; ++i) {
        err = dichotomy_method(epsilons[i], equation[i], left_borders[i], right_borders[i], &res);
        printf("%d: ", i + 1);
        if (err)
            printf("Wrong Interval or Invalid Function\n");
        else
            printf("root is: %Lf.16\n", res);
    }
    return 0;
}
