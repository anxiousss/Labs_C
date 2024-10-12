
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

struct WordsBuffer {
    int size, capacity;
    char** words;
    int* word_sizes;
    int* word_capacities;
};

typedef struct WordsBuffer WordsBuffer;

int len(const char* str);

int base_conversion(int number, char* result, int based);

int HandlerOptR(char** paths);

int HandlerOptA(char** paths);

WordsBuffer* create_buffer();

int insert_symbol(WordsBuffer* buffer, char c);

int insert_word(WordsBuffer* buffer);

int split_by_words(WordsBuffer* buffer, FILE* file);

void free_buffer(WordsBuffer* buffer);


#endif //SOLUTION_H~
