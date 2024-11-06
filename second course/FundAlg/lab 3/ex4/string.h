
#ifndef EX3_4_STRING_H
#define EX3_4_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

typedef struct String {
  char* mas;
  int size;
  int capacity;
} String;

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

int len(const char* str);

int init_string(String* string, char* src);

void delete_string(String* string);

int cmp_string(String* string1, String* string2);

int eq_string(String* string1, String* string2);

int copy_str(String* dst, String* src);

int copy_newstr(String* dst, String* src);

int concatenation(String* A, String* B);

#endif //EX3_4_STRING_H
