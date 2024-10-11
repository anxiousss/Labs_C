#include "solution.h"


int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}


int base_conversion(int number, char* result, int based) {
    if (based < 2 || based > 36) {
        result[0] = '\0';
        return Invalid_input;
    }

    if (number == 0) {
        result[0] = '0';
        result[1] = '\0';
        return Invalid_input;
    }

    char tmp[65];
    int index = 0;

    while (number > 0) {
        int remainder = number % based;
        if (remainder < 10) {
            tmp[index] = '0' + remainder;
        } else {
            tmp[index] = 'A' + (remainder - 10);
        }
        index++;
        number /= based;
    }

    for (int i = 0; i < index; i++) {
        result[i] = tmp[index - i - 1];
    }
    result[index] = '\0';
    return 0;
}


int split(FILE* file, char*** buffer, int* word_index, int* char_index, int* buffer_size, int* word_size) {
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c != ' ' && c != '\n' && c != '\t') {
            if (*word_index >= *buffer_size)  {
                *buffer_size *= 2;
                char** tmp1 = (char** )realloc(*buffer, sizeof(char*) * *buffer_size);
                if (tmp1 == NULL) {
                    free(tmp1);
                    return Memory_leak;
                }
                *buffer = tmp1;
                for (int i = *word_index; i < *buffer_size; ++i) {
                    (*buffer)[i] = (char* ) malloc(*word_size * sizeof(char));
                }
            }
            if (*char_index >= *word_size) {
                *word_size *= 2;
                char* tmp2 = (char* )realloc((*buffer)[*word_index], sizeof(char) * (*word_size));
                if (tmp2 == NULL) {
                    free(tmp2);
                    return Memory_leak;
                }
                (*buffer)[*word_index] = tmp2;
                for (int i = *char_index; i < *word_size; ++i) {
                    (*buffer)[*word_index][i] = ' ';
                }
            }
            (*buffer)[*word_index][*char_index] = c;
            (*char_index)++;
        } else {
            if (*char_index > 0) {
                (*buffer)[*word_index][*char_index] = '\0';
                (*word_index)++;
                *char_index = 0;
            }
        }
    }
    if (*char_index > 0) {
        (*buffer)[*word_index][*char_index] = '\0';
        (*word_index)++;
    }
    return 0;
}

int buffer_alloc(char*** buffer, int buffer_size, int word_size) {
    *buffer = (char**)malloc(buffer_size * sizeof(char*));
    if (*buffer == NULL) {
        return Memory_leak;
    }
    for (int i = 0; i < buffer_size; ++i) {
        (*buffer)[i] = (char*)malloc(word_size * sizeof(char));
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


int HandlerOptR(char** paths) {
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

    int buffer_size1 = 10, word_size1 = 5;
    int alloc_res1 = buffer_alloc(&buffer1, buffer_size1, word_size1);

    if (alloc_res1) {
        fclose(fin1);
        fclose(fin2);
        fclose(out);
        return alloc_res1;
    }

    char** buffer2 = NULL;
    int  buffer_size2 = 10, word_size2 = 5;
    int alloc_res2 = buffer_alloc(&buffer2, buffer_size2, word_size2);

    if (alloc_res2) {
        fclose(fin1);
        fclose(fin2);
        fclose(out);
        return alloc_res2;
    }

    int word_index1 = 0, word_index2 = 0, char_index1 = 0, char_index2 = 0;
    int res1 = split(fin1, &buffer1, &word_index1, &char_index1, &buffer_size1, &word_size1);
    int res2 = split(fin2, &buffer2, &word_index2, &char_index2, &buffer_size2, &word_size2);

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


    int max_words = word_index1 > word_index2 ? word_index1 : word_index2;

    for (int i = 0; i < max_words; ++i) {

        if (i < word_index1 && i < word_index2) {
            if (i % 2 == 0) {
                fprintf(out, "%s ", buffer1[i]);
            } else {
                fprintf(out, "%s ", buffer2[i]);
            }
        }
        else if (i < word_index1) {
            fprintf(out, "%s ", buffer1[i]);
        }
        else if (i < word_index2) {
            fprintf(out, "%s ", buffer2[i]);
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

int HandlerOptA(char** paths) {
    if (strcmp(paths[0], paths[1]) == 0) {
        return Equal_paths;
    }

    FILE* fin1 = fopen(paths[0], "r");
    FILE* out = fopen(paths[1], "w");

    if (!fin1 || !out) {
        if (fin1) fclose(fin1);
        if (out) fclose(out);
        return Memory_leak;
    }

    char** buffer1 = NULL;
    int buffer_size1 = 10, word_size1 = 5;
    int alloc_res1 = buffer_alloc(&buffer1, buffer_size1, word_size1);

    if (alloc_res1) {
        fclose(fin1);
        fclose(out);
        return alloc_res1;
    }

    int word_index1 = 0, char_index1 = 0;
    int res1 = split(fin1, &buffer1, &word_index1, &char_index1, &buffer_size1, &word_size1);

    if (res1) {
        fclose(fin1);
        fclose(out);
        for (int i = 0; i < buffer_size1; ++i) {
            free(buffer1[i]);
        }
        free(buffer1);
        return Memory_leak;
    }

    // logic

    for (int i = 0; i < buffer_size1; ++i) {
        if (i % 10 == 0) {
            for (int j = 0; buffer1[i][j] != '\0'; ++j) {
                if (buffer1[i][j] >= 'A' && buffer1[i][j] <= 'Z')
                    buffer1[i][j] = 'a' + (buffer1[i][j] - 'A');
            }
            for (int j = 0; j < buffer1[i][j] != '\0'; ++j) {
                char result[65];
                int err = base_conversion(buffer1[i][j], result, 4);
                if (err) {
                    return err;
                }
                fprintf(out, "%s", result);
                putc(' ', out);
            }
        } else if (i % 2 == 0) {
            for (int j = 0; buffer1[i][j] != '\0'; ++j) {
                if (buffer1[i][j] >= 'A' && buffer1[i][j] <= 'Z')
                    buffer1[i][j] = 'a' + (buffer1[i][j] - 'A');
            }
            fprintf(out, "%s ", buffer1[i]);
        } else if (i % 5 == 0) {
            for (int j = 0; j < buffer1[i][j] != '\0'; ++j) {
                char result[65];
                int err = base_conversion(buffer1[i][j], result, 8);
                if (err) {
                    return err;
                }
                fprintf(out, "%s", result);
                putc(' ', out);
            }
        }  else {
            fprintf(out, "%s ", buffer1[i]);
        }
    }

    for (int i = 0; i < buffer_size1; ++i) {
        free(buffer1[i]);
    }
    free(buffer1);


    fclose(fin1);
    fclose(out);

    return 0;
}
