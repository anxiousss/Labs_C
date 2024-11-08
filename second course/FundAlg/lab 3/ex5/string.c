#include "string.h"

int len(const char* str) {
    int i = 0;
    while (str[++i] != '\0');
    return i;
}

int init_string(String* string, char* src) {
    if (src == NULL) {
        string->size = 0;
        string->capacity = 5;
    } else {
        string->size = len(src);
        string->capacity = string->size + 5;
    }
    string->mas = (char*)(malloc(sizeof(char) * string->capacity));
    if (!string->mas)
        return Memory_leak;

    for (int i = 0; i < string->size; ++i) {
        string->mas[i] = src[i];
    }
    string->mas[string->size] = '\0';
    return 0;
}

void delete_string(String* string) {
    if (string->mas)
        free(string->mas);

    string->mas = NULL;
    string->size = 0;
    string->capacity = 0;
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
    delete_string(dst);
    dst->mas = (char *)(calloc(src->capacity, sizeof(char )));
    if (!dst->mas)
        return Memory_leak;
    dst->size = src->size;
    char* tempdst = dst->mas;
    char* tempsrc =  src->mas;
    while ((*dst->mas++ = *src->mas++));\
    dst->mas = tempdst;
    src->mas = tempsrc;
}

int copy_newstr(String* dst, String* src) {
    int err = init_string(dst, NULL);
    if (err)
        return err;
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