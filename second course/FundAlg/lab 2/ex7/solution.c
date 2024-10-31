#include "solution.h"

int dichotomy_method(long double eps, long double(*func)(long double ), long double a, long double b, long double* res) {
    if (func(a) * func(b) >  0) {
        return Wrong_interval;
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

// root = 2 on [1, 3]
long double eq1(long double x) {
    return x * x - 4.0;
}
// root = pi on [0, pi]
long double eq2(long double x) {
    return sinl(x);
}

// root = 1 on [0, 1.5]
long double eq3(long double x) {
    return powl(x, 3.0) - 2.0 * powl(x, 2.0) - x + 2.0;
}

// root = ln2 on [0, 1]
long double eq4(long double x) {
    return expl(x) - 2;
}

// root = e on [1, 3]
long double eq5(long double x) {
    return logl(x) - 1;
}

// no roots on [-2, 2]
long double eq6(long double x) {
    return x * x + 1;
}

// root = 1 on [0, 2]
long double eq7(long double x) {
    return powl(x - 1.0, 2.0);
}

// root -> 1 on [0, 2]
long double eq8(long double x) {
    return 1.0/(x - 1);
}

// root = 0.5 on [0.1, 1]
long double eq9(long double x) {
    return 1.0/x - 2;
}

// root = pi/3 on [0, pi]
long double eq10(long double x) {
    return cosl(x) - 0.5;
}
