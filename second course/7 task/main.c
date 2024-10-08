#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int GetOpts(int argc, char** argv, kOpts* option, const char** paths) {
    if (argc != 3) {
        return 1;
    }

    for (int i = 1; i <= 2; ++i) {
        const char *procceding_option = argv[i];
        int number_argc = 0;
        if (procceding_option[0] == '/' || procceding_option[0] == '-') {
            switch (procceding_option[1]) {
                case 'r':
                    *option = OPT_R;
                    number_argc = 5;
                    break;
                case 'a':
                    *option = OPT_A;
                    number_argc = 4;
                    break;
                default:
                    return 1;
            }
        }
        else {
            for (int j = i; j < number_argc + 1 ; ++j) {
                if (len(argv[j]) > PATH_MAX) {
                    return Invalid_input;
                }
                paths[j - i] = argv[j];
            }
        }
    }
    return 0;
}


int main(int argc, char** argv) {
    kOpts opt = 1;
    const char** paths = malloc(10 * sizeof(char*));
    if (paths == NULL) {
        return Memory_leak  ;
    }
    int result = GetOpts(argc, argv, &opt, paths);

    if (result) {
        return result;
    }

    int (*handlers[2])(const char**) = {
            HandlerOptR,
            HandlerOptA
    };

    free(paths);
}
