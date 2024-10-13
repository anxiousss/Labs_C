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

    solution(&in, &out);
    return 0;
}
