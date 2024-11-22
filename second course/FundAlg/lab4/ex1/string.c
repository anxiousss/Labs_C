#include "string.h"

int len(const char* str) {
    int i = 0;
    while (str[++i] != '\0');
    return i;
}
int init_string(String* string, char** src) {
    if (src == NULL) {
        string->size = 0;
        string->capacity = 5;
        string->mas = (char*)malloc(sizeof(char) * string->capacity);
        if (!string->mas)
            return Memory_leak;
        string->mas[0] = '\0'; // Ensure it's a valid empty string
        return 0;
    } else {
        string->size = len(*src);
        string->capacity = string->size + 5;
        string->mas = (char*)malloc(sizeof(char) * string->capacity);
        if (!string->mas)
            return Memory_leak;
        strncpy(string->mas, *src, string->capacity);
        string->mas[string->capacity - 1] = '\0'; // Ensure null-termination
        return 0;
    }
}

void delete_string(String* string) {
    if (string->mas)
        free(string->mas);
}

int cmp_string(String* string1, String* string2) {
    if (string1->size > string2->size)
        return 1;
    else if (string1->size < string2->size)
        return -1;

    char* tmp1 = string1->mas;
    char* tmp2 = string2->mas;
    while (*tmp1 && *tmp2) {
        if (*tmp1 > *tmp2) return 1;
        if (*tmp1 < *tmp2) return -1;
        ++tmp1;
        ++tmp2;
    }
    if (*tmp1) return 1;
    if (*tmp2) return -1;
    return 0;
}

int eq_string(String* A, String* B) {
    if (cmp_string(A, B) == 0)
        return 1;
    return 0;
}
int copy_str(String* dst, String* src) {
    delete_string(dst); // Free existing memory
    dst->capacity = src->capacity;
    dst->size = src->size;
    dst->mas = (char*)malloc(sizeof(char) * dst->capacity);
    if (!dst->mas)
        return Memory_leak;
    strncpy(dst->mas, src->mas, dst->capacity);
    dst->mas[dst->capacity - 1] = '\0'; // Ensure null-termination
    return 0;
}
int copy_newstr(String* dst, String* src) {
    init_string(dst, NULL);
    dst->size = src->size;
    char* tempdst = dst->mas;
    char* tempsrc = src->mas;
    while ((*dst->mas++ = *src->mas++));
    dst->mas = tempdst;
    src->mas = tempsrc;
    return 0;
}


int concatenation(String* A, String* B) {
    int new_size = A->size + B->size;

    if (new_size >= A->capacity) {
        A->capacity = new_size + 5;
        char* new_mas = (char*)realloc(A->mas, sizeof(char) * A->capacity);
        if (!new_mas) {
            return Memory_leak;
        }
        A->mas = new_mas;
    }

    for (int i = 0; i < B->size; ++i) {
        A->mas[A->size + i] = B->mas[i];
    }

    A->size = new_size;
    A->mas[A->size] = '\0';

    return 0;
}