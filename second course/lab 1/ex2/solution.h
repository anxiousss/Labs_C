
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <math.h>

typedef enum Errors {
    Invalid_input = 2,
    Overflow,
    Equal
} Errors;

int my_atof(const char* string, long double* result);

long long int factorial(int number);
long double combinations(int n, int k);

long double calculate_limit(long double eps, long double (*func)(int));
long double series_sum(long double eps, long double(*func)(long double, int), int multiplier, int n);
int dichotomy_method(long double, long double(*func)(long double ), long double, long double, long double*);
long double compute_limit_sqrt2(long double eps);
long double compute_series_sqrt2(long double eps);
long double compute_limit_pi(long double eps);
int newton_method(long double eps, long double a, long double b, long double* res, long double(*f)(long double), long double(*fd1)(long double), long double(*fd2)(long double));


long double limit_e(int n );
long double series_e(long double part, int n);
long double equation_e(long double x);

long double series_pi(long double part, int n);
long double equation_pi(long double x);
long double equation_pi_deriative1(long double x);
long double equation_pi_deriative2(long double x);


long double limit_ln2(int n);
long double series_ln2(long double part, int n);
long double equation_ln2(long double x);

long double limit_sqrt2(long double prev);
long double series_sqrt2(int n);
long double equation_sqrt2(long double x);

long double limit_gamma(int n);
long double series_gamma(long double part, int n);

#endif //SOLUTION_H
