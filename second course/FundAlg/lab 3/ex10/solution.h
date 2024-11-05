
#ifndef EX3_10_SOLUTION_H
#define EX3_10_SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak,
    Equal_Paths
} Errors;

typedef struct Node {
    char value;
    struct Node* children;
    struct Node* next;
} Node;

int check_file_names(const char *file1, const char *file2);

Node* create_node(char value);

void add_child(Node* parent, Node* child);

Node* build_tree(const char* expression);

void destroy(Node* root);

void print_tree(Node* root, FILE* file, int depth);

char* readLineFromFile(FILE* file);

#endif //EX3_10_SOLUTION_H
