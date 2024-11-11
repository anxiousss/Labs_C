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
        case Invalid_id:
            printf("Invalid id");
        default:
            return 0;
    }
}