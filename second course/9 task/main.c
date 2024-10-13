#include "solution.h"

int main(int argc, char** argv) {
    if (argc != 3)
        return Invalid_input;

    int a = 0, b = 0;
    int err = my_atoi(argv[1], &a) | my_atoi(argv[2], &b);

    if (err)
        return err;

    task1(a, b);

    task2();
    return 0;
}
