#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hex.h"

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;


int flag_recognizing(char* str, const char* flag1, const char* flag2) {
    return strcmp(str, flag1) == 0 && strcmp(str, flag2);
}


int GetOpts(int argc, char** argv, kOpts* option, int* number) {
    if (argc != 3) {
        return 1;
    }

    for (int i = 1; i <= 2; ++i) {
        const char* procceding_option = argv[i];
        if (procceding_option[0] == '/' || procceding_option[0] == '-') {
            switch (procceding_option[1])
            {
            case 'h':
                *option = OPT_H;
                break;
            case 'p':
                *option = OPT_P;
                break;
            case 's':
                *option = OPT_S;
                break;
            case 'e':
                *option = OPT_E;
                break;
            case 'a':
                *option = OPT_A;
                break;
            case 'f':
                *option = OPT_F;
                break;
            default:
                return 1;
                break;
            }
        }
        else {
            for (int j = 0; procceding_option[j]; ++j) {
                if (procceding_option[j] >= '0' && procceding_option[j] <= '9') {
                    *number *= 10;
                    *number += procceding_option[j] - '0';
                }
                else {
                    return 1;
                }
            }
        }
    }
    return 0;
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

void HandlerOptE(int number) {
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
        HandlerOptE,
        HandlerOptA
    };

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("%s", "Incorrect option");
        return 1;
    }

    handlers[opt](procceed_number);
    return 0;
}
