#include <stdio.h>
#include "Tree.h"

int main() {
    Tree* root = NULL;
    root = insert(root, 0);
    root = insert(root, 1);
    root = insert(root, 3);
    root = insert(root, 4);
    root = insert(root, 6);
    root = insert(root, 10);
    root = insert(root, -1);
    root = insert(root, -2);
    root = insert(root, -3);

    tree_print(root);
    printf("\n%d", VertexCount(root));
    destroy(root);
}
