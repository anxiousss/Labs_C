#include "solution.h"

int GetOpts(int argc, char** argv, kOpts* opt, char* in_path, char* out_path) {
    if (argc < 2 || argc > 4) {
        printf("Invalid input");
        return Invalid_input;
    }

    const char* proceeding_option = argv[1];
    if (proceeding_option[0] == '/' || proceeding_option[0] == '-') {
        char flag = proceeding_option[1];
        strcpy(in_path, argv[2]);

        if (flag == 'n' && argc == 4) {
            flag = proceeding_option[2];
            strcpy(out_path, argv[3]);
        } else if (flag != 'n' && argc == 3) {
            strcat(out_path, in_path);
        } else {
            printf("Invalid input");
            return Invalid_input;
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
                printf("Invalid input");
                return Invalid_input;
        }
    } else {
        printf("Invalid input");
        return Invalid_input;
    }

    if (check_file_names(in_path, out_path)) {
        printf("Equal Paths");
        return Equal_paths;
    }

    return 0;
}
int main(int argc, char** argv) {
    kOpts opt = 1;
    char out_path[PATH_MAX] = "_out", in_path[PATH_MAX];

    int result = GetOpts(argc, argv, &opt, in_path, out_path);
    if (result) {
        return result;
    }

    int (*handlers[4])(char*, char*) = {HandlerOptD, HandlerOptI, HandlerOptS, HandlerOptA};

    result = handlers[opt](in_path, out_path);
    switch (result) {
        case Invalid_input:
            printf("Invalid input");
            break;
        case Equal_paths:
            printf("Equal Paths");
            break;
        case Memory_leak:
            printf("Memory leak");
            break;
        default:
            return result;
    }
}
