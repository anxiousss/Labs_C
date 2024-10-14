#include "solution.h"

int main(int argc, char** argv) {
    if (argc != 3)
        return Invalid_input;

    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    if (!in || !out) {
        if (!in) fclose(in);
        if (!out) fclose(out);
        return Memory_leak;
    }

    int res = solution(&in, &out);
    switch (res) {
        case Invalid_input:
            printf("Invalid input\n");
            return res;
        case Memory_leak:
            printf("Memory leak\n");
            return res;
        case Equal_paths:
            printf("Equal Paths");
            return res;
        default:
            return res;
    }
}
