#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum Errors {
    Success = 0,
    Invalid_input = 1,
    Memory_allocation_error = 2,
    File_open_error = 3
} Errors;

struct Search {
    int num_line;
    int num_char;
    int pos;
};

typedef struct Search Search;

struct Vector {
    Search* data;
    int capacity;
    int size;
};

typedef struct Vector Vector;

Vector* init_vector(int size);
void free_vector(Vector* v);
void push(Vector* v, Search search);
void update(Vector* v, const char* sub, int ch);
void check_search(Vector* v, int len, char** results, int* result_count, char* file);
int FindFiles(char* sub, int amount, char*** results, int* total_results, ...);

#endif // SOLUTION_H