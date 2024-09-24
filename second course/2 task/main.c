#include <stdio.h>
#include <math.h>

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T
} kOpts;

int GetOpts(int argc, char** argv, kOpts* option, double* eps, double* arr) {
    int number_argc = 0;
    for (int i = 1; i <= 2; ++i) {
        const char* procceding_option = argv[i];
        if (procceding_option[0] == '/' || procceding_option[0] == '-') {
            switch (procceding_option[1])
            {
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
        }
        else {
            if (number_argc == argc - 2) {
                switch (*option) {
                    case OPT_Q:
                    case OPT_T:
                        *eps = procceding_option[i + 1];
                        for (int j = i + 2; j < j + number_argc; ++j) {
                            *(arr + j - i - 2) = procceding_option[j];
                        }
                    case OPT_M:
                        for (int j = i + 1; j < number_argc; ++j) {
                            *(arr + j - j - 1) = procceding_option[j];
                        }
                }
            } else {
                return 1;
            }
        }
    }
    return 0;
}

void HandlerOptQ(double eps, double* arr) {
    printf("%f\n", eps);
    for (int i = 0; i < 4; ++i) {
        printf("%f", arr[i]);
    }
}

int main(int argc, char** argv) {
    kOpts opt = 0;
    double eps = 0.0;
    double* arr[4];

    void (*handlers[3])(double , double*) = {
            HandlerOptQ,
    };

    if (GetOpts(argc, argv, &opt, &eps, arr)) {
        printf("%s", "Incorrect option");
        return 1;
    }

    handlers[opt](eps, arr);
    return 0;
}