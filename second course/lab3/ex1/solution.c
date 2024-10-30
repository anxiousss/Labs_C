#include "solution.h"

int add(int a, int b) {
    int carry;
    while (b != 0){
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int negative(int n) {
    return add(~n, 1);
}

int elder_bit(int n) {
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    return add(n, negative(n >> 1));
}

void translate_to_system(int dec, char* result, int r) {
    if (dec == 0){
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    int mask = (1 << r) - 1;
    int a = 0;
    char * alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    int length = 0;
    if (!elder_bit(dec)) {
        a = -dec;
        result[0] = '-';
        length = (int)(log2(a) / log2(1 << r)) + 2;
    } else {
        a = dec;
        length = (int)(log2(a) / log2(1 << r)) + 1;
    }
    result[length] = '\0';
    while (a > 0) {
        result[--length] = alphabet[a & mask];
        a = a >> r;
    }
}