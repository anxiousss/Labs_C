#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hex.h"

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_C,
    OPT_A,
    OPT_F
} kOpts;


int flag_recognizing(char* str, const char* flag1, const char* flag2) {
    return strcmp(str, flag1) == 0 && strcmp(str, flag2);
}


int GetOpts(int argc, char** argv, kOpts* option, int* number) {
    if (argc < 2) {
        return 1;
    }

    *number = atoi(argv[1]);
    if (flag_recognizing(argv[2], "-h", "/h")) {
        *option = OPT_H;
        return 0;
    }
    else if (flag_recognizing(argv[2], "-p", "/p")) {
        *option = OPT_P;
        return 0;
    }
    else if (flag_recognizing(argv[2], "-s", "/s")) {
        *option = OPT_S;
        return 0;
    }
    else if (flag_recognizing(argv[2], "-c", "/c")) {
        *option = OPT_C;
        return 0;
    }
    else if (flag_recognizing(argv[2], "-a", "/a")) {
        *option = OPT_A;
        return 0;
    }
    else if (flag_recognizing(argv[2], "-f", "/f")) {
        *option = OPT_F;
        return 0;
    }

    return 1;
}


void HandlerOptH(int number) {
    int flag = 0;
    for (int x = 1; x < 101; ++x) {
        if (x % number == 0) {
            flag = 1;
            printf("%d ", x);
        }
    }

    if (!flag) {
        printf("No numbers");
    }
}

void HandlerOptP(int number) {
    int flag = 0;
    for (int x = 2; x < (int)sqrt(number) + 1; ++x) {
        if (number % x == 0) {
            printf("composite number");
            return;
        }
    }
    printf("simple number");
}
void HandlerOptS(int number) {
    int len = 0;
    int digits[33];
    int discharges[9];
    hex(bin(number, &len, digits), len, discharges);
}

void HandlerOptC(int number) {
    for (int x = 1; x < number + 1; ++x) {
        for (int d = 1; d < 11; ++d) {
            printf("%f ", pow(x, d));
        }
        printf("\n");
    }
}

void HandlerOptA(int number) {
    printf("%d", ((1 + number) / 2) * (number + 1));
}


int main(int argc, char** argv) {
    kOpts opt = OPT_H;
    int procceed_number = 0;
    void (*handlers[6])(int) = {
        HandlerOptH,
        HandlerOptP,
        HandlerOptS,
        HandlerOptC,
        HandlerOptA
    };

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("%s", "Incorrect option");
        return 1;
    }

    handlers[opt](procceed_number);
    return 0;
}
