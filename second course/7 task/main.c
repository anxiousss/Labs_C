#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int GetOpts(int argc, char** argv, kOpts* option, const char** paths) {
    if (argc != 4 && argc != 5) {
        return 1;
    }

    int path_index = 0;
    for (int i = 1; i < argc; ++i) {
        const char *procceding_option = argv[i];
        if (procceding_option[0] == '/' || procceding_option[0] == '-') {
            switch (procceding_option[1]) {
                case 'r':
                    *option = OPT_R;
                    break;
                case 'a':
                    *option = OPT_A;
                    break;
                default:
                    return 1;
            }
        } else {
            if (len(argv[i]) > PATH_MAX) {
                return Invalid_input;
            }
            paths[path_index++] = argv[i];
        }
    }

    // Проверка, что все пути были считаны
    if (path_index != 3) {
        return Invalid_input;
    }

    return 0;
}

int main(int argc, char** argv) {
    kOpts opt = 1;
    const char** paths = malloc(3 * sizeof(char*));
    if (paths == NULL) {
        return Memory_leak;
    }
    int result = GetOpts(argc, argv, &opt, paths);

    if (result) {
        free(paths);
        return result;
    }

    int (*handlers[2])(const char**) = {
            HandlerOptR,
            HandlerOptA
    };

    int handler_result = handlers[opt](paths);

    free(paths);
    return handler_result;
}
