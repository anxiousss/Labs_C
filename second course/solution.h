
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int len(const char*);

int convert_to_decimal(const char *number, int base, char* convert_number);

int find_min_base(const char *number);

int my_toupper(int c);

WordsBuffer* create_buffer();

int insert_symbol(WordsBuffer* buffer, char c);

int insert_word(WordsBuffer* buffer);

int split_by_words(WordsBuffer* buffer, FILE* file);

void free_buffer(WordsBuffer* buffer);

int solution(FILE** in, FILE** out);

#endif //SOLUTION_H
