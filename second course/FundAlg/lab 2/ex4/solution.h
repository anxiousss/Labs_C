

#ifndef EX2_4_SOLUTION_H
#define EX2_4_SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

typedef struct Coord {
    double x;
    double y;
} Coord;

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

double pseudo_scalar_product(Coord a, Coord b, Coord c);

int is_convex(int n, ...);

double polynomial(double x, int n, ...);

int is_kaprekar(int base, char* number);

int kaprekar_handler(int base, char*** kaprekar_numbers, int n, ...);


#endif //EX2_4_SOLUTION_H
