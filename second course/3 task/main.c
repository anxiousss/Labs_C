#include <stdio.h>
#include "solution.h"

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T
} kOpts;


double my_atof(const char* str) {
    double result = 0.0;
    double fraction = 1.0;
    int sign = 1;
    int i = 0;

    if (str[i] == '-') {
        sign = -1;
        ++i;
    }


    while (str[i] != '.' && str[i] != '\0') {
        result = result * 10.0 + (str[i] - '0');
        ++i;
    }

    if (str[i] == '.') {
        ++i;
    }

    while (str[i] != '\0') {
        fraction /= 10.0;
        result += (str[i] - '0') * fraction;
        ++i;
    }

    return sign * result;
}

int GetOpts(int argc, char** argv, kOpts* option, double* eps, double* arr) {
    int number_argc = 0;
    for (int i = 1; i <= 2; ++i) {
        const char* procceding_option = argv[i];
        if (procceding_option[0] == '/' || procceding_option[0] == '-') {
            switch (procceding_option[1]) {
                case 'q':
                    *option = OPT_Q;
                    number_argc = 4;
                    break;
                case 'm':
                    *option = OPT_M;
                    number_argc = 2;
                    break;
                case 't':
                    *option = OPT_T;
                    number_argc = 4;
                    break;
                default:
                    return 1;
            }
        } else {
            if (number_argc == argc - 2) {
                switch (*option) {
                    case OPT_Q:
                    case OPT_T:
                        *eps = my_atof(argv[i]);
                        for (int j = i + 1; j < i + number_argc; ++j) {
                            arr[j - i - 1] = my_atof(argv[j]);
                        }
                        break;
                    case OPT_M:
                        for (int j = i; j < i + number_argc; ++j) {
                            arr[j - i] = my_atof(argv[j]);
                        }
                        break;
                }
            } else {
                return 1;
            }
        }
    }
    return 0;
}

void HandlerOptQ(double eps, double* arr) {
    permute(arr, 0, 2, eps, eq_solution);
}

void HandlerOptM(double eps, double* arr) {
    if ((int)arr[0] % (int)arr[1] == 0) {
        printf("multiples ");
    } else {
        printf("not multiples ");
    }
}

void HandlerOptT(double eps, double* arr) {
    permute(arr, 0, 2, eps, triangle_check);
}


int main(int argc, char** argv) {
    kOpts opt = 0;
    double eps = 0.0;
    double arr[4];

    void (*handlers[3])(double, double*) = {
            HandlerOptQ,
            HandlerOptM,
            HandlerOptT
    };

    if (GetOpts(argc, argv, &opt, &eps, arr)) {
        printf("%s\n", "Incorrect option");
        return 1;
    }

    handlers[opt](eps, arr);
    return 0;
}
