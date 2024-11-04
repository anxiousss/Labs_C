#include "solution.h"

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Invalid input");
        return Invalid_input;
    }
    char in_path[PATH_MAX];
    char out_path[PATH_MAX];
    strcpy(in_path, argv[1]);
    strcpy(out_path, argv[2]);


    FILE* in = fopen(in_path, "r");
    FILE* out = fopen(out_path, "w");
    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
        printf("Invalid input\n");
        return Invalid_input;
    }

    Node* root;
    char* expression;
    while ((expression = readLineFromFile(in)) != NULL) {
        int index = 0;
        root = build_tree(expression, &index);
        if (!root) {
            printf("Memory leak");
            destroy(root);
            return Memory_leak;
        }
        printTree(out, root, 0);
        fprintf(out, "-------------------------------------------------\n");

        free(expression);
        destroy(root);
    }

    fclose(in);
    fclose(out);
    return 0;
}
