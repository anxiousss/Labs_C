#include "solution.h"

int main() {
    int res = dialog_manager();
    switch (res) {
        case Wrong_input:
            printf("Wrong input");
            return Wrong_input;
        case Memory_leak:
            printf("Memory leak");
            return Memory_leak;
        case Invalid_code:
            printf("Invalid secret code");
            return Invalid_code;
        case File_error:
            printf("File error");
            return File_error;
        default:
            return 0;
    }
}