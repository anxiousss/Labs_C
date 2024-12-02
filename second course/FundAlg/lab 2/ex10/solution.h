

#ifndef EX2_10_SOLUTION_H
#define EX2_10_SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef enum Errors {
    Memory_leak = 2
} Errors;


int expansion_series(double eps, double a, double **result, int n, ...);

#endif //EX2_10_SOLUTION_H
