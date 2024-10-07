#include <stdio.h>
#include "solution.h"

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T
} kOpts;

typedef enum Errors {
    Overflow = 2,
    Invalid_number,
} Errors;


int my_atof(const char *string, double *result) {
    int sign = 1;
    double number = 0.0;
    double fraction = 0.0;
    int has_fraction = 0;
    int fraction_digits = 0;

    for (int j = 0; string[j] != '\0'; ++j) {
        char c = string[j];

        if (c == '-' && j == 0) {
            sign = -1;
        } else if (c == '.' && !has_fraction) {
            has_fraction = 1;
        } else if (c >= '0' && c <= '9') {
            if (has_fraction) {
                fraction = fraction * 10.0 + (c - '0');
                fraction_digits++;
            } else {
                number = number * 10.0 + (c - '0');
            }
        } else {
            return Invalid_number;
        }
    }

    while (fraction_digits > 0) {
        fraction /= 10.0;
        fraction_digits--;
    }

    *result = sign * (number + fraction);
    return 0;
}

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int GetOpts(int argc, char** argv, kOpts* option, double* eps, double* arr1, int* arr2) {
    int number_argc = 0;
    int flag = 0;
    for (int i = 1; i <= 2; ++i) {
        const char *procceding_option = argv[i];
        if ((procceding_option[0] == '/' || procceding_option[0] == '-') && flag == 0 && len(procceding_option) == 2) {
            flag = 1;
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
                        int err = my_atof(argv[i], eps);
                        if (err) {
                            return err;
                        }
                        for (int j = i + 1; j < i + number_argc; ++j) {
                            err = my_atof(argv[j], arr1 + j - i - 1);
                            if (err) {
                                return err;
                            }
                        }
                        break;
                    case OPT_M:
                        for (int k = i; k < i + number_argc; ++k) {
                            int sign = 1;
                            procceding_option = argv[k];
                            for (int j = 0; procceding_option[j]; ++j) {
                                if (procceding_option[j] == '-' && sign == 1)
                                    sign = -1;
                                else if (procceding_option[j] >= '0' && procceding_option[j] <= '9') {
                                    arr2[k - 2] *= 10;
                                    arr2[k - 2] = (procceding_option[j] - '0');
                                } else {
                                    return Invalid_number;
                                }
                            }
                            arr2[k - 2] *= sign;
                            if (arr2[k - 2] == 0) {
                                return Invalid_number;
                            }
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

int HandlerOptQ(double eps, double* arr) {
    permute(arr, 0, 2, eps, eq_solution);
    return 0;
}

int HandlerOptM(int* arr) {
    if (arr[0] % arr[1] == 0) {
        printf("multiplies");
    } else {
        printf("not multiplies");
    }
    return 0;
}

int HandlerOptT(double eps, double* arr) {
    if (arr[0] > 0 && arr[1] > 0 && arr[2] > 0) {
        triangle_check(arr[0], arr[1], arr[2], eps);
        return 0;
    }
    return Invalid_number;
}


int main(int argc, char** argv) {
    kOpts opt = 0;
    double eps = 0.0;
    double arr1[3];
    int arr2[2];
    int (*handlers[2])(double, double*) = {
            HandlerOptQ,
            HandlerOptT
    };

    int result = GetOpts(argc, argv, &opt, &eps, arr1, arr2);
    if (result != 0) {
        return result;
    }
    if (opt == OPT_M) {
        return HandlerOptM(arr2);
    } else {
        return handlers[opt](eps, arr1);
    }
}
