
#ifndef SOLUTION_H
#define SOLUTION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
typedef enum Errors {
    Invalid_input = 2
} Errors;

int len(const char* str);

int convert(int old_base, int new_base, char* string, char* result);


#endif //SOLUTION_H
