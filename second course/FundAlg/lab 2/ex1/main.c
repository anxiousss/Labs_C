#include "solution.h"

int GetOpts(int argc, char** argv, kOpts* opt) {
    const char *flag = argv[1];
    if ((flag[0] == '/' || flag[0] == '-') && len(flag) == 2) {
        switch (flag[1]) {
            case 'l':
                if (argc != 3) return Invalid_input;
                *opt = OPT_L;
                break;
            case 'r':
                if (argc != 3) return Invalid_input;
                *opt = OPT_R;
                break;
            case 'u':
                if (argc != 3) return Invalid_input;
                *opt = OPT_U;
                break;
            case 'n':
                if (argc != 3) return Invalid_input;
                *opt = OPT_N;
                break;
            case 'c':
                if (argc < 4) return Invalid_input;
                *opt = OPT_C;
                break;
            default:
                return Invalid_input;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    kOpts opt;
    int res = GetOpts(argc, argv, &opt);
    if (res) {
        printf("Invalid input\n");
        return res;
    }
    int size;
    char* str;
    unsigned int seed;
    switch (opt) {
        case OPT_L:
            res = HandlerOptL(argv, &size);
            printf("%d", size);
            return res;
        case OPT_R:
            res = HandlerOptR(argv, &str);
            if (res) {
                printf("Memory leak");
                return res;
            }
            printf("%s", str);
            free(str);
            return res;
        case OPT_U:
            res = HandlerOptU(argv, &str);
            if (res) {
                printf("Memory leak");
                return res;
            }
            printf("%s", str);
            free(str);
            return res;
        case OPT_N:
            res = HandlerOptN(argv, &str);
            if (res) {
                printf("Memory leak");
                return res;
            }
            printf("%s", str);
            free(str);
            return res;
        case OPT_C:
            res = HandlerOptC(argc, argv, &str, &seed);
            switch (res) {
                case Invalid_input:
                    printf("Invalid input");
                    return res;
                case Memory_leak:
                    printf("Memory leak");
                    return res;
                default:
                    break;
            }
            printf("%s", str);
            free(str);
            return res;
    }
}
