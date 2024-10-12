#include "solution.h"

void hex(int x, char* result, int size) {
    char letters[16] = "0123456789ABCDEF";
    int i = size - 1;

    while (x > 0 && i >= 0) {
        result[i--] = letters[x % 16];
        x /= 16;
    }
}

int HandlerOptD(char* in, char* out) {
    FILE* fin1 = fopen(in, "r");
    FILE* fin2 = fopen(out, "w");

    if (!fin1 || !fin2) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        return Memory_leak;
    }
    int c;
    while (!feof(fin1)) {
        c = fgetc(fin1);
        if (c > 0 && !(c >= '0' && c <= '9'))
            fputc(c, fin2);
    }
    fclose(fin1);
    fclose(fin2);
    return 0;
}

int HandlerOptI(char* in, char* out) {
    FILE* fin1 = fopen(in, "r");
    FILE* fin2 = fopen(out, "w");

    if (!fin1 || !fin2) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        return Memory_leak;
    }

    int c, latin_letters = 0, line_number = 0, flag = 0;
    while (!feof(fin1)) {
        c = fgetc(fin1);
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            ++latin_letters;
            flag = 1;
        }
        else if (c == '\n') {
            flag = 0;
            ++line_number;
            fprintf(fin2, "String %d contains %d latin letters\n", line_number, latin_letters);
            latin_letters = 0;
        }
    }

    if (flag)
        fprintf(fin2, "String %d contains %d latin letters\n", line_number + 1, latin_letters);


    fclose(fin1);
    fclose(fin2);
    return 0;
}

int HandlerOptS(char* in, char* out) {
    FILE* fin1 = fopen(in, "r");
    FILE* fin2 = fopen(out, "w");

    if (!fin1 || !fin2) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        return Memory_leak;
    }

    int c, symbols = 0, line_number = 0;
    while (!feof(fin1)) {
        c = fgetc(fin1);
        if (c != -1 && !(((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))  || (c >= '0' && c <= '9') || c == ' ') && c != '\n') {
            ++symbols;
        } else if (c == '\n' || c == -1) {
            ++line_number;
            fprintf(fin2, "String %d contains %d unique symbols\n", line_number, symbols);
            symbols = 0;
        }
    }

    fclose(fin1);
    fclose(fin2);
    return 0;
}

int HandlerOptA(char* in, char* out) {
    FILE* fin1 = fopen(in, "r");
    FILE* fin2 = fopen(out, "w");

    if (!fin1 || !fin2) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        return Memory_leak;
    }
    int c;
    while (!feof(fin1)) {
        c = fgetc(fin1);
        if (c != -1 && !(c >= '0' && c <= '9')) {
            int size = (int)(log2(c) * 0.25) + 1;
            char result[size];
            hex(c, result, size);
            fprintf(fin2, "%s", result);
        } else if (c != -1)
            fputc(c, fin2);
    }
    fclose(fin1);
    fclose(fin2);
    return 0;
}

int check_file_names(const char *file1, const char *file2) {
    const char * name1 = strrchr(file1,'\\');
    const char * name2 = strrchr(file2,'\\');
    if (name1 != NULL) {
        name1++;
    } else {
        name1 = file1;
    }
    if (name2 != NULL) {
        name2++;
    } else {
        name2 = file2;
    }

    if (strcmp(name1, name2) == 0) {
        return 1;
    }
    return 0;
}
