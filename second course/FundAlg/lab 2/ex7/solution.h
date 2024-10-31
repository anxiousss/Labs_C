

#ifndef SOLUTION_H
#define SOLUTION_H
#include <stdio.h>
#include <math.h>

typedef enum Errors {
    Wrong_interval = 2
} Errors;

int dichotomy_method(long double eps, long double(*func)(long double ), long double a, long double b, long double* res);


long double eq1(long double x);

long double eq2(long double x);

long double eq3(long double x);

long double eq4(long double x);

long double eq5(long double x);

long double eq6(long double x);

long double eq7(long double x);

long double eq8(long double x);

long double eq9(long double x);

long double eq10(long double x);

#endif //SOLUTION_H
