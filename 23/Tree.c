#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"

Tree* insert(Tree* node, int item) {
    if (node == NULL) {
        node = (Tree*)malloc(sizeof(Tree));
        node->value = item;
        node->left = node->right = NULL;
        return node;
    }
    else if (item < node->value) {
        node->left = insert(node->left, item);
    }
    else if (item > node->value) {
        node->right = insert(node->right, item);
    }
    return node;
}

Tree* search(Tree* node, int item) {
    if (node == NULL || item == node->value) {
        return node;
    }
    else if (item < node->value) {
        return search(node->left, item);
    }
    else {
        return search(node->right, item);
    }
}

void destroy(Tree* node) {
    if (node != NULL) {
        destroy(node->left);
        destroy(node->right);
        free(node);
    }
}

void order(Tree* node) {
    if (node != NULL) {
        order(node->left);
        printf("%d ", node->value);
        order(node->right);
    }
}

Tree* dtree(Tree* root, int key) {
    Tree* p, * p2;
    if (!root)
        return root;
    if (root->value == key) {
        if (root->left == root->right) {
            free(root);
            return NULL;
        }
        else if (root->left == NULL) {
            p = root->right;
            free(root);
            return p;
        }
        else if (root->right == NULL) {
            p = root->left;
            free(root);
            return p;
        }
        else {
            p2 = root->right;
            p = root->right;
            while (p->left)
                p = p->left;
            p->left = root->left;
            free(root);
            return p2;
        }
    }
    if (root->value < key)
        root->right = dtree(root->right, key);
    else
        root->left = dtree(root->left, key);
    return root;
}


void visualizeTree(Tree* root, int space) {
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    visualizeTree(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->value);

    // Process left child
    visualizeTree(root->left, space);
}

int VertexCount(Tree* root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + VertexCount(root->left) + VertexCount(root->right);
}