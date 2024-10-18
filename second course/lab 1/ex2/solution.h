
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <math.h>

typedef enum Errors {
    Invalid_input = 2,
} Errors;

int my_atof(const char* string, long double* result);


long double calculate_limit(long double eps, long double (*func)(int));
long double series_sum(long double eps, long double(*func)(long double, int), int multiplier, int n);
int dichotomy_method(long double, long double(*func)(long double ), long double, long double, long double*);
long double compute_limit_sqrt2(long double);
long double compute_series_sqrt2(long double);
long double compute_limit_pi(long double eps);


long double limit_e(int);
long double series_e(long double, int);
long double equation_e(long double);

long double series_pi(long double, int);
long double equation_pi(long double);

long double limit_ln2(int);
long double series_ln2(long double, int);
long double equation_ln2(long double);

long double limit_sqrt2(long double);
long double series_sqrt2(int );
long double equation_sqrt2(long double);

#endif //SOLUTION_H
