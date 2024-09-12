#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_C,
    OPT_A,
    OPT_F
} kOpts;

int GetOpts(int argc, char** argv, kOpts *option, int *number) {
    if (argc < 2) {
        // Если нет аргументов, считаем это ошибкой
        return 1;
    }

    *number = atoi(argv[1]);
    if (strcmp(argv[2], "-h") == 0) {
        *option = OPT_H;
        return 0;
    } else if (strcmp(argv[2], "-p") == 0) {
        *option = OPT_P;
        return 0;
    } else if (strcmp(argv[2], "-s") == 0) {
        *option = OPT_S;
        return 0;
    } else if (strcmp(argv[2], "-c") == 0) {
        *option = OPT_C;
        return 0;
    } else if (strcmp(argv[2], "-a") == 0) {
        *option = OPT_A;
        return 0;
    } else if (strcmp(argv[2], "-f") == 0) {
        *option = OPT_F;
        return 0;
    }
    
    return 1;
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
    for (int x = 2; x < number / 2 + 1; ++x) {
        if (number % x == 0) {
            flag = 1;
            printf("composite number");
            break;
        }
    }
    if (!flag) {
        printf("simple number");
    }
}
void HandlerOptS(int number) {
    
}
void HandlerOptC(int number);
void HandlerOptA(int number);
void HandlerOptF(int number);


int main (int argc, char** argv) {
    kOpts opt = 0;
    int procceed_number = 0;
    void (*handlers[6])(int) = {
        HandlerOptH,
        HandlerOptP,
        HandlerOptS
    };

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("%s", "Incorrect option");
        return 1;
    }

    handlers[opt](procceed_number);
    return 0;
}