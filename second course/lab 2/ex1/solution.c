#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

void reverseString(char *str) {
    int length = len(str);
    char *start = str;
    char *end = str + length - 1;
    char temp;

    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;

        ++start;
        --end;
    }
}


int my_atoi(const char* str, unsigned int* number) {
    *number = 0;
    for (int i = 0; str[i]; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            *number *= 10;
            *number += (str[i] - '0');
        }
        else {
            return Invalid_input;
        }
    }
    return 0;
}

void string_copy(char *dst, const char *src) {
    char *dstaddr = dst;
    while((*dst++ = *src++));
}

char* my_strcat(char *dest, const char *src) {
    char *ptr = dest + len(dest);

    while (*src != '\0') {
        *ptr++ = *src++;
    }

    *ptr = '\0';

    return dest;
}


int is_num(int x) {
    return x >= '0' && x <= '9';
}

int is_letter(int x) {
    return (x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z');
}

int is_alnum(int x) {
    return is_num(x) || is_letter(x);
}



int my_toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}

int HandlerOptL(char** argv, int* size) {
    *size = len(argv[2]);
    return 0;
}

int HandlerOptR(char** argv, char** str) {
    int string_size = len(argv[2]);
    *str = (char* ) malloc(sizeof(char) * (string_size + 1));
    string_copy(*str, argv[2]);
    if (!*str) return Memory_leak;
    reverseString(*str);
    return 0;
}

int HandlerOptU(char** argv, char** str) {
    int string_size = len(argv[2]);
    *str = (char* ) malloc(sizeof(char) * (string_size + 1));
    if (!*str) return Memory_leak;
    string_copy(*str, argv[2]);
    for (int i = 0; (*str)[i] != '\0'; ++i) {
        if (i % 2 == 1) {
            (*str)[i] = (char) my_toupper((*str)[i]);
        }
    }
    (*str)[string_size] = '\0';
    return 0;
}

int HandlerOptN(char** argv, char** str) {
    int string_size = len(argv[2]);
    *str = (char* )(malloc(sizeof(char) * (string_size + 1)));
    if (!*str) return Memory_leak;
    string_copy(*str, argv[2]);
    int index = 0;
    for (int i = 0; argv[2][i] != '\0'; ++i) {
        if (is_num(argv[2][i])) {
            (*str)[index] = argv[2][i];
            ++index;    
        }
    }

    for (int i = 0; argv[2][i] != '\0'; ++i) {
        if (is_letter(argv[2][i])) {
            (*str)[index] = argv[2][i];
            ++index;
        }
    }

    for (int i = 0; argv[2][i] != '\0'; ++i) {
        if (!is_alnum(argv[2][i])) {
            (*str)[index] = argv[2][i];
            ++index;
        }
    }
    (*str)[index] = '\0';
    return 0;
}

int HandlerOptC(int argc, char** argv, char** str, unsigned int* seed) {
    int err = my_atoi(argv[3], seed);
    if (err)
        return err;
    int all_string_size  = len(argv[2]);
    srand(*seed);
    for (int i = 4; i < argc; ++i) {
        all_string_size += len(argv[i]);
    }
    *str = (char* ) malloc(sizeof(char) * (all_string_size + 1));
    if (!*str) return Memory_leak;
    (*str)[0] = '\0';
    for (int j = 3; j < argc - 1; ++j) {
        argv[j] = argv[j + 1];
    }
    int size = argc - 3, random_index;
    for (int i = 0; i < argc - 3; ++i) {
        random_index = rand() % size + 2;
        *str = my_strcat(*str, argv[random_index]);
        for (int j = random_index; j < argc - 1; ++j) {
            argv[j] = argv[j + 1];
        }
        --size;
    }
    return 0;
}