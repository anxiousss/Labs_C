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

    //code
    return 0;
}