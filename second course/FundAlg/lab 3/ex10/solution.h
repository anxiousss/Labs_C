
#ifndef EX3_10_SOLUTION_H
#define EX3_10_SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

typedef struct Node {
    char* value;
    struct Node** mas;
    int size;
    int capacity;
} Node;



Node* init(char* value);

int addChild(Node* parent, Node* child);

Node* build_tree(char* str, int* index);

void destroy(Node* root);

void printTree(FILE* fin, Node* root, int level);

char* readLineFromFile(FILE* file);

#endif //EX3_10_SOLUTION_H
