#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;

typedef enum Errors {
    Overflow = 2,
    Invalid_number,
    No_numbers
} Errors;

int GetOpts(int argc, char** argv, kOpts* option, int* number) {
    int digits[100];

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
            }
        }
        else {
            int j = 0;
            for (j; procceding_option[j]; ++j) {
                if (procceding_option[j] >= '0' && procceding_option[j] <= '9') {
                    digits[j] = procceding_option[j] - '0';
                    *number *= 10;
                    *number += digits[j];
                }
                else {
                    return Invalid_number;
                }
            }
            if (j > 10) {
                return Overflow;
            }
        }
    }
    return 0;
}

int HandlerOptH(int number) {
    int flag = 0, x = number;
    if (x == 0) {
        return Invalid_number;
    }
    while (number < 101) {
        if (number % x == 0) {
            flag = 1;
            printf("%d  ", number);
        }
        number += x;
    }
    if (!flag) {
        return No_numbers;
    }
    return 0;
}

int HandlerOptP(int number) {

    if (number <= 1) {
        return Invalid_number;
    }

    for (int x = 2; x <= sqrt(number); ++x) {
        if (number % x == 0) {
            printf("composite number");
            return 0;
        }
    }
    printf("simple number");
    return 0;
}



int HandlerOptS(int number) {
    if (number == 0) {
        printf("%d", 0);
    } else {
        int residue = 0, i = 1;
        int digits[9];
        while (number != 0 ) {
            residue = number % 16;
            if (residue < 10) {
                residue += 48;
            } else {
                residue += 57;
            }
            digits[i] = residue;
            ++i;
            number /= 16;
        }
        for (int j = i - 1; j > 0; --j) {
            printf("%c", digits[j]);
        }
    }
    return 0;
}

int HandlerOptE(int number) {
    if (number == 0) {
        return Invalid_number;
    }

    for (int x = 1; x < number + 1; ++x) {
        for (int d = 1; d < 11; ++d) {
            printf("%f ", pow(x, d));
        }
        printf("\n");
    }
    return 0;
}

int HandlerOptA(int number) {
    if (number == 0) {
        return Invalid_number;
    }
    printf("%d", ((1 + number) / 2) * (number + 1));
    return 0;
}

int HandlerOptF(int number) {
    long long factor = 1;
    for (int x = 2; x < number + 1; ++x) {
        if (factor > LLONG_MAX / x) {
            return Overflow;
        }
        factor *= x;
    }

    printf("%lld", factor);
    return 0;
}


int main(int argc, char** argv) {
    kOpts opt = OPT_H;
    int procceed_number = 0;
    int (*handlers[6])(int) = {
            HandlerOptH,
            HandlerOptP,
            HandlerOptS,
            HandlerOptE,
            HandlerOptA,
            HandlerOptF
    };

    int result = GetOpts(argc, argv, &opt, &procceed_number);
    if (result != 0) {
        return result;
    }
    return handlers[opt](procceed_number);
}
