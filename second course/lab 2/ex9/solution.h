
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

int gcd(int a, int b);

int is_prime(int n);

int fractional_part_to_int(double number, int* numerator, int* denominator);

void prime_factors(int* factors, int number, int* index);

int is_finite(char*** results, int base, int n, ...);

#endif //SOLUTION_H
