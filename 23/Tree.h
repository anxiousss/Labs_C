#ifndef TREE_H
#define TREE_H


struct Tree {
    int value;
    struct Tree* left;
    struct Tree* right;
};

typedef struct Tree Tree;

Tree* insert(Tree* node, int item);

Tree* search(Tree* node, int item);

void destroy(Tree* node);

void order(Tree* node);

Tree* dtree(Tree* root, int key);

void tree_print(Tree* node);

void tree_print_with_depth(Tree* root, int depth);

int VertexCount(Tree* root);

#endif