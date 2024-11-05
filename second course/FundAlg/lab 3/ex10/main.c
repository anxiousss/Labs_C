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

    if (check_file_names(in_path, out_path)) {
        printf("Equal Paths");
        return Equal_Paths;
    }


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
        root = build_tree(expression);
        if (!root) {
            printf("Memory leak");
            destroy(root);
            free(expression);

            return Memory_leak;
        }
        print_tree(root,out, 0);
        fprintf(out, "-------------------------------------------------\n");
        free(expression);
        destroy(root);
    }

    fclose(in);
    fclose(out);
    return 0;
}