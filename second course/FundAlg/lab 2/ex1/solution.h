

#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdlib.h>

typedef enum kOpts {
    OPT_L = 1,
    OPT_R,
    OPT_U,
    OPT_N,
    OPT_C
} kOpts;

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

int len(const char* str);

void reverseString(char *str);

void string_copy(char *dst, const char *src);

int my_atoi(const char* str, unsigned int* number);

int my_toupper(int c);

int is_num(int x);

int is_alnum(int x);

int is_letter(int x);

int HandlerOptL(char** argv, int* size);

int HandlerOptR(char** argv, char** str );

int HandlerOptU(char** argv, char** str);

int HandlerOptN(char** argv, char** str);

int HandlerOptC(int argc, char** argv, char** str, unsigned int* seed);

#endif //SOLUTION_H
