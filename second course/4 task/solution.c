#include "solution.h"



int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int HandlerOptD(char* in, char* out) {
    printf("%s", in);
    printf("%s", out);
    return 0;
}

int HandlerOptI(char* in, char* out) {
    return 0;
}

int HandlerOptS(char* in, char* out) {
    return 0;
}

int HandlerOptA(char* in, char* out) {
    return 0;
}
