#include "solution.h"

int GetOpts(int argc, char** argv, kOpts* opt, char* in, char* out) {
    if (argc < 2 || argc > 4) {
        return Invalid_input;
    }

    const char* procceding_option = argv[1];
    if (procceding_option[0] == '/' || procceding_option[0] == '-') {
        char flag = procceding_option[1];
        in = argv[2];
        if (flag == 'n') {
            flag = procceding_option[2];
            out = argv[3];
        } else {
            out = strcpy(out + 4, in);
        }
        switch (flag) {
            case 'd':
                *opt = OPT_D;
                break;
            case 'i':
                *opt = OPT_I;
                break;
            case 's':
                *opt = OPT_S;
                break;
            case 'a':
                *opt = OPT_A;
                break;
            default:
                return Invalid_input;
        }
    } else {
        return Invalid_input;
    }

    if (strcmp(in, out) == 0) {
        return Equal_paths;
    }

    return 0;
}

int main(int argc, char** argv) {
    kOpts opt = 1;
    char out_path[PATH_MAX] = "_out", in_path[PATH_MAX];

    int result = GetOpts(argc, argv, &opt, out_path, in_path);
    if (result) {
        return result;
    }

    int (*handlers[4])(char*, char*) = {HandlerOptD, HandlerOptI, HandlerOptS, HandlerOptA};

    return handlers[opt](in_path, out_path);
}

