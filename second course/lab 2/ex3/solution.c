#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int my_strncmp(const char *str1, const char *str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

int line_break_count(const char* str) {
    int c = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '\n')
            ++c;
    }
    return c;
}

int find(int* string_numerous, int* char_numerous, const char* sub, FILE* fin, int* rep) {
    int l = len(sub);
    int char_index = 0, string_index = 1, line_breaks = line_break_count(sub), sub_start = 0, index = 0;
    char buffer[l + 1];
    char ch;
    int i = 0;

    while ((ch = fgetc(fin)) != EOF) {
        buffer[i] = ch;
        i++;

        ++char_index;
        sub_start = char_index;
        if (ch == '\n') {
            ++string_index;
            char_index = line_breaks > 0 ? sub_start : 1;
        }

        if (i == l) {
            buffer[i] = '\0';
            if (my_strncmp(sub, buffer, l) == 0) {
                string_numerous[index] = string_index - line_breaks;
                char_numerous[index] = (line_breaks > 0 ? sub_start + 1 : char_index) - l ;
                ++index;
            }

            for (int j = 0; j < l - 1; j++) {
                    buffer[j] = buffer[j + 1];
            }
            i--;
        }
    }
    *rep = index;
    return 0;
}



int find_sub_files(char*** result, const char* substr, int* reps , ...) {
    va_list paths;
    va_start(paths, reps);
    char* file = va_arg(paths, char*);
    int last = 0;
    while (file != NULL) {

        int* string_numerous = (int*)(malloc(sizeof(int) * 1024));
        if (!string_numerous)
            return Memory_leak;
        int* char_numerous = (int*)(malloc(sizeof(int) * 1024));
        if (!char_numerous) {
            free(string_numerous);
            return Memory_leak;
        }
        FILE* fin = fopen(file, "r");
        if (!fin) {
            fclose(fin);
            return Memory_leak;
        }
        int rep = 0;
        find(string_numerous, char_numerous, substr, fin, &rep);
        *result = (char**)(realloc(*result, sizeof(char*) * rep));
        for (int i = last; i < rep + last; ++i) {
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "FILE %s, string %d, symbol %d", file, string_numerous[i - last], char_numerous[i - last]);
            (*result)[i] = strdup(buffer);
        }
        last += rep;

        file = va_arg(paths, char*);
        free(string_numerous);
        free(char_numerous);
        fclose(fin);
    }
    *reps = last;
    va_end(paths);
    return 0;
}
