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

void visualizeTree(Tree* node, int depth);

int VertexCount(Tree* root);

#endif