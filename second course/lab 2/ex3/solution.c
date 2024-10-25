#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

Vector* init_vector(int size) {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (!vector)
        return NULL;
    vector->size = 0;
    vector->capacity = size;
    vector->data = (Search*)malloc(sizeof(Search) * size);
    if (!vector->data) {
        free(vector);
        return NULL;
    }

    return vector;
}

void free_vector(Vector* v) {
    if (v) {
        if (v->data) {
            free(v->data);
        }
        free(v);
    }
}

void push(Vector* v, Search search) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        Search* old = v->data;
        v->data = (Search*)malloc(sizeof(Search) * v->capacity);
        if (!v->data) {
            free(old);
            return;
        }
        for (int i = 0; i < v->size; i++)
            v->data[i] = old[i];
        free(old);
    }

    v->data[v->size] = search;
    v->size++;
}

void update(Vector* v, const char* sub, int ch) {
    for (int i = 0; i < v->size; i++) {
        int j = v->data[i].pos;
        if (sub[j] == ch) {
            v->data[i].pos++;
        } else {
            v->data[i].pos = -1;
        }
    }
}

void check_search(Vector* v, int len, char*** result, char* file) {
    int pos = 0;
    for (int i = 0; i < v->size; i++) {
        if (v->data[i].pos == len) {
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "FILE %s Line number: %3d, char number: %3d\n", file, v->data[i].num_line, v->data[i].num_char);
            (*result) = (char**)realloc(*result, (pos + 1) * sizeof(char*));
            if (!(*result)) {
                return;
            }
            (*result)[pos] = strdup(buffer);
            if (!(*result)[pos]) {
                return;
            }
            pos++;
        } else if (v->data[i].pos != -1) {
            v->data[pos] = v->data[i];
            pos++;
        }
    }
    v->size = pos;
}

int find(char*** result, char* file, const char* sub) {
    Vector* v = init_vector(10);
    if (!v) {
        return Memory_leak;
    }

    int l = len(sub);
    int num_char = 0, num_line = 1;

    int ch;
    FILE* fin = fopen(file, "r");
    if (!fin) {
        free_vector(v);
        return Memory_leak;
    }

    while ((ch = fgetc(fin)) != EOF) {
        num_char++;

        if (ch == sub[0]) {
            Search search = {num_line, num_char, 0};
            push(v, search);
        }

        update(v, sub, ch);
        check_search(v, l, result, file);

        if (ch == '\n') {
            num_line++;
            num_char = 0;
        }
    }

    free_vector(v);
    fclose(fin);
    return 0;
}

int find_sub_files(char*** result, const char* substr, ...) {
    va_list files;
    va_start(files, substr);
    char* K = va_arg(files, char*);
    while (K != NULL) {
        int err = find(result, K, substr);
        if (err) {
            return err;
        }
        K = va_arg(files, char*);
    }
    va_end(files);
    return 0;
}