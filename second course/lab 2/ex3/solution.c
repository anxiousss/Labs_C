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

size_t read_string(FILE *file, char *buffer, size_t len) {
    size_t bytes_read = fread(buffer, 1, len, file);
    if (bytes_read > 0) {
        fseek(file, -len + 1, SEEK_CUR);
    }
    return bytes_read;
}

int find_all(int** string_numer, int** char_numer, int* count, const char* sub, FILE* fin) {
    int l = len(sub);
    int char_index = 0, string_index = 1, n_line_breaks = 0;
    char buffer[l + 1];
    *count = 0;
    while (!feof(fin)) {
        size_t bytes_read = read_string(fin, buffer, l);
        if (bytes_read == 0) break;
        buffer[bytes_read] = '\0';
        n_line_breaks = line_break_count(buffer);
        if (n_line_breaks != 0)
            char_index = 0;
        string_index += n_line_breaks;
        n_line_breaks = 0;

        if (my_strncmp(sub, buffer, l) == 0) {
            (*string_numer)[*count] = string_index;
            (*char_numer)[*count] = char_index;
            (*count)++;
        }

        ++char_index;
    }

    return (*count > 0) ? 0 : 1;
}

    

int find_sub_files(int* string_numerous,int* char_numerous , const char* substr, char* file, ...) {
    va_list ap;
    va_start(ap, file);



}