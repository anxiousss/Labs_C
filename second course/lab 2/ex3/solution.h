
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;


int len(const char*);

int my_strncmp(const char *str1, const char *str2, size_t n);

int line_break_count(const char* str);

int find(int* string_numer, int* char_numer, const char* sub, FILE* fin, int* rep);

int find_sub_files(char*** result, const char* substr, int* reps , ...);

#endif //SOLUTION_H
