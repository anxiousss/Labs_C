#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>

typedef enum kOpts {
    OPT_D,
    OPT_I,
    OPT_S,
    OPT_A
} kOpts;

typedef enum Errors {
    Invalid_input = 2,
    Equal_paths,
    Memory_leak
} Errors;

void hex(int x, char* result, int size);

int HandlerOptD(char* in, char* out);

int HandlerOptI(char* in, char* out);

int HandlerOptS(char* in, char* out);

int HandlerOptA(char* in, char* out);

int check_file_names(const char *file1, const char *file2);
#endif //SOLUTION_H
