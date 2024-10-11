

#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef enum kOpts {
    OPT_D,
    OPT_I,
    OPT_S,
    OPT_A
} kOpts;

typedef enum Errors {
    Invalid_input = 2,
    Equal_paths
} Errors;

int len(const char* str);

int HandlerOptD(char* in, char* out);

int HandlerOptI(char* in, char* out);

int HandlerOptS(char* in, char* out);

int HandlerOptA(char* in, char* out);


#endif //SOLUTION_H
