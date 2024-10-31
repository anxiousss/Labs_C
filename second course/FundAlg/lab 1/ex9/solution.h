
#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

int cmp_func(const void* a, const void* b);

int my_atoi(const char* str, int* number);

void swap(int *a, int *b);

int binary_search(int* arr, int value, int size);

int task1(int a, int b);

int task2();

void print_arr(int* arr, int size);

#endif //SOLUTION_H
