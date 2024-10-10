
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

typedef enum kOpts {
    OPT_R,
    OPT_A
} kOpts;

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak,
    Equal_paths
} Errors;

int len(const char* str);

int base_conversion(int number, char* result, int based);

int split(FILE* file, char*** buffer, int* word_index, int* char_index, int* buffer_size, int* word_size);

int HandlerOptR(char** paths);

int HandlerOptA(char** paths);

int buffer_alloc(char*** buffer, int buffer_size, int word_size);

#endif //SOLUTION_H
