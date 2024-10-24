#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

int len(const char* str);

void remove_leading_zeros(const char* number, char* result);

int addition(int base, char* a, char* b, char* result);

int sum(int base, int n, char** result, ...);

#endif //SOLUTION_H
