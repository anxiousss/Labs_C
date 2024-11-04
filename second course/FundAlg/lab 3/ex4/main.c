#include "solution.h"


int main() {
    int result = dialog_manager();
    switch (result) {
        case Invalid_input:
            printf("Invalid input");
            return result;
        case Memory_leak:
            printf("Memory leak");
            return result;
        default:
            return 0;
    }
}