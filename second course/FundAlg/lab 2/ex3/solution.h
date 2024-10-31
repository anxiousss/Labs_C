
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

int len(const char* str);

Vector* init_vector(int size);

void free_vector(Vector* v);

void push(Vector* v, Search search);

void update(Vector* v, const char* sub, int ch);

void check_search(Vector* v, int len, char*** result, char* file);

int find(char*** result, char* file, const char* sub);

int find_sub_files(char*** result, const char* substr, ...);


#endif //SOLUTION_H
