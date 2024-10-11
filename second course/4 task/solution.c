#include "solution.h"



int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int HandlerOptD(char* in, char* out) {
    FILE* fin1 = fopen(in, "r");
    FILE* fin2 = fopen(out, "w");

    if (!fin1 || !fin2) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        return Memory_leak;
    }
    int c;
    while (!feof(fin1)) {
        c = fgetc(fin1);
        if (c > 0 && !(c >= '0' && c <= '9'))
            fputc(c, fin2);
    }
    fclose(fin1);
    fclose(fin2);
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
