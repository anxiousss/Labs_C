#include "solution.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid input");
        return Invalid_input;
    }
    char path[PATH_MAX];
    strcpy(path, argv[1]);
    FILE* fin = fopen(path, "r+");
    if (!fin) {
        printf("Invalid_input");
        return Invalid_input;
    }
    int result = text_replace(fin);
    switch (result) {
        case Invalid_input:
            printf("Invalid input");
            return Invalid_input;

        case Memory_leak:
            printf("Memory leak");
            return Memory_leak;

        default:
            return 0;
    }
}
