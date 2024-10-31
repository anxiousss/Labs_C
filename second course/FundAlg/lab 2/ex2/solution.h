
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <math.h>
#include <stdarg.h>

typedef enum Errors {
    Invalid_input = 2
} Errors;

double geometric_mean(int n, ...);

double recurse_exp(double x, int n);

#endif //SOLUTION_H
