#include "solution.h"

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
    free(v->data);
    free(v);
}

void push(Vector* v, Search search) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        Search* old = v->data;
        v->data = (Search*)malloc(sizeof(Search) * v->capacity);
        if (!v->data) {
            free(old);
            free(v);
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

void check_search(Vector* v, int len, char** results, int* result_count, char* file) {
    int pos = 0;
    for (int i = 0; i < v->size; i++) {
        if (v->data[i].pos == len) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "FILE %s Line number: %d, char number: %d\n", file, v->data[i].num_line, v->data[i].num_char);
            results[(*result_count)++] = strdup(buffer);
        } else if (v->data[i].pos != -1) {
            v->data[pos] = v->data[i];
            pos++;
        }
    }
    v->size = pos;
}

char** FindFiles(char* sub, int amount, int* total_results, ...) {
    va_list files;
    va_start(files, total_results);
    char** results = (char**)malloc(sizeof(char*) * 1000);
    if (!results)
        return NULL;
    *total_results = 0;

    for (int i = 0; i < amount; i++) {
        char* filename = va_arg(files, char*);
        FILE* file = fopen(filename, "r");
        if (!file) {
            free(results);
            return NULL;
        }
        Vector* v = init_vector(10);
        if (!v) {
            fclose(file);
            free(results);
            return NULL;
        }
        int l = strlen(sub);

        int num_line = 1;
        int num_char = 0;

        int ch;
        while ((ch = fgetc(file)) != EOF) {
            num_char++;
            if (ch == sub[0]) {
                Search search = {num_line, num_char, 0};
                push(v, search);
            }
            update(v, sub, ch);
            check_search(v, l, results, total_results, filename);
            if (ch == '\n') {
                num_line++;
                num_char = 0;
            }
        }
        free_vector(v);
        fclose(file);
    }
    va_end(files);
    return results;
}

