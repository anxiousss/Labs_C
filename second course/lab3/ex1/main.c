#include "solution.h"



int main() {
    char result[34];
    translate_to_system(-65547, result, 4);
    printf("%s", result);
    return 0;
}
