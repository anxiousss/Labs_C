#include "solution.h"

int min(int a, int b) {
    return a < b ? a : b;
}


int split(FILE* file, char** buffer, int* word_index, int* char_index, int* buffer_size, int* word_size) {
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c != ' ' && c != '\n' && c != '\t') {
            if (*word_index >= *buffer_size)  {
                *buffer_size *= 2;
                buffer = realloc(buffer, sizeof(char*) * *buffer_size);
                if (buffer == NULL) {
                    return Memory_leak;
                }
            }
            if (*char_index >= *word_size) {
                *word_size *= 2;
                buffer[*word_index] = realloc(buffer[*word_index], sizeof(char) * *word_size);
                if (buffer[*word_index] == NULL) {
                    return Memory_leak;
                }
            }
            buffer[*word_index][*char_index] = c;
            (*char_index)++;
        } else if (c == '\n') {
            continue;
        } else {
            buffer[*word_index][*char_index] = '\0';
            (*word_index)++;
            *char_index = 0;
        }
    }
    if (*char_index > 0) {
        buffer[*word_index][*char_index] = '\0';
        (*word_index)++;
    }
    return 0;
}

int buffer_alloc(char*** buffer, int buffer_size, int word_size) {
    *buffer = malloc(buffer_size * sizeof(char*));
    if (*buffer == NULL) {
        return Memory_leak;
    }
    for (int i = 0; i < buffer_size; ++i) {
        (*buffer)[i] = malloc(word_size * sizeof(char));
        if ((*buffer)[i] == NULL) {
            for (int j = 0; j < i; ++j) {
                free((*buffer)[j]);
            }
            free(*buffer);
            return Memory_leak;
        }
    }
    return 0;
}
int HandlerOptR(const char** paths) {
    if (strcmp(paths[0], paths[1]) == 0) {
        return Equal_paths;
    }

    FILE* fin1 = fopen(paths[0], "r");
    FILE* fin2 = fopen(paths[1], "r");
    FILE* out = fopen(paths[2], "w");

    if (!fin1 || !fin2 || !out) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        if (out) fclose(out);
        return Memory_leak;
    }

    char** buffer1 = NULL;
    char** buffer2 = NULL;
    int buffer_size1 = 10, buffer_size2 = 10, word_size1 = 5, word_size2 = 5;
    int alloc_res1 = buffer_alloc(&buffer1, buffer_size1, word_size1);
    int alloc_res2 = buffer_alloc(&buffer2, buffer_size2, word_size2);

    if (alloc_res1) {
        fclose(fin1);
        fclose(fin2);
        fclose(out);
        return alloc_res1;
    }

    if (alloc_res2) {
        fclose(fin1);
        fclose(fin2);
        fclose(out);
        // Освобождаем уже выделенную память для buffer1
        for (int i = 0; i < buffer_size1; ++i) {
            free(buffer1[i]);
        }
        free(buffer1);
        return alloc_res2;
    }

    int word_index1 = 0, word_index2 = 0, char_index1 = 0, char_index2 = 0;
    int res1 = split(fin1, buffer1, &word_index1, &char_index1, &buffer_size1, &word_size1);
    int res2 = split(fin2, buffer2, &word_index2, &char_index2, &buffer_size2, &word_size2);

    if (res1 || res2) {
        fclose(fin1);
        fclose(fin2);
        fclose(out);
        // Освобождаем уже выделенную память для buffer1 и buffer2
        for (int i = 0; i < buffer_size1; ++i) {
            free(buffer1[i]);
        }
        free(buffer1);
        for (int i = 0; i < buffer_size2; ++i) {
            free(buffer2[i]);
        }
        free(buffer2);
        return Memory_leak;
    }
    // Здесь можно добавить логику обработки файлов

    int end = min(buffer_size1, buffer_size2);
    for (int i = 0; i < end; ++i) {
        if (i % 2 == 0) {
            fprintf(out, "%s", buffer1[i]);
        } else {
            fprintf(out, "%s", buffer2[i]);
        }
    }


    // Освобождаем память
    for (int i = 0; i < buffer_size1; ++i) {
        free(buffer1[i]);
    }
    free(buffer1);
    for (int i = 0; i < buffer_size2; ++i) {
        free(buffer2[i]);
    }
    free(buffer2);

    fclose(fin1);
    fclose(fin2);
    fclose(out);
    return 0;
}

int HandlerOptA(const char** paths) {
    return 0;
}
