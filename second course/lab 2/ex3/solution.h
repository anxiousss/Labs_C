
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;


int len(const char*);

int my_strncmp(const char *str1, const char *str2, size_t n);

int line_break_count(const char* str);

size_t read_string(FILE *file, char *buffer, size_t len);

int find(int* string_numer, int* char_numer, const char* sub, FILE* fin);

int find_sub_files(int* string_numerous, int* char_numerous, const char* substr, char* file, ...);

#endif //SOLUTION_H
