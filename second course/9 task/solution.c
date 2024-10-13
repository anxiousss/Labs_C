#include "solution.h"

int cmp_func(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int my_atoi(const char* str, int* number) {
    int sign = 1;
    for (int i = 0; str[i] ; ++i) {
        if (str[i] == '-' && sign == 1)
            sign = -1;
        else if (str[i] >= '0' && str[i] <= '9') {
            *number *= 10;
            *number += (str[i] - '0');
        }
        else {
            return Invalid_input;
        }
    }
    *number *= sign;
    return 0;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void print_arr(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n\n\n");
}

int binary_search(int* arr, int value, int size) {
    int closest;
    int left = 0, right = size - 1, mid;

    while (left <= right) {
        mid = (left + right) / 2;
        if (abs(arr[mid] - value) < abs(arr[closest] - value))
            closest = mid;

        if (arr[mid] == value)
            return mid;

        if (arr[mid] < value)
            left = mid + 1;

        else if (arr[mid] > value)
            right = mid - 1;

    }
    return closest;
}

int task1(int a, int b) {

    if (a < 0 || b < 0)
        return Invalid_input;

    if (a > b)
        swap(&a, &b);

    int max = INT_MIN, min = INT_MAX, min_ind = 0, max_ind = 0;
    int size = 10;
    int arr[size];

    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        arr[i] = a + rand() % (b - a + 1);
        if (arr[i] > max) {
            max = arr[i];
            max_ind = i;
        }
        if (arr[i] < min) {
            min = arr[i];
            min_ind = i;
        }
    }

    print_arr(arr, size);
    printf("%d %d\n\n\n", min, max);
    swap(arr + min_ind, arr + max_ind);
    print_arr(arr, size);

    return 0;
}

int task2() {
    int size_A = 10 + rand() % (10000 - 10 + 1);
    int size_B = 10 + rand() % (10000 - 10 + 1);

    int* A = (int*)(malloc(sizeof(int) * size_A));
    if (A == NULL) {
        return Memory_leak;
    }

    int* B = (int*)(malloc(sizeof(int) * size_B));
    if (B == NULL) {
        free(A);
        return Memory_leak;
    }

    for (int i = 0; i < size_A; ++i) {
        A[i] = -1000 + rand() % (1000 + 1000 + 1);
    }

    for (int i = 0; i < size_B; ++i) {
        B[i] = -1000 + rand() % (1000 + 1000 + 1);
    }

    int* C = (int*)malloc(sizeof(int) * size_A);
    if (C == NULL) {
        free(A);
        free(B);
        return Memory_leak;
    }
    qsort(B, size_B, sizeof(int), cmp_func);

    for (int i = 0; i < size_A; ++i) {
        C[i] = A[i] + binary_search(B, A[i], size_B);
    }

    printf("Array A\n");
    print_arr(A, size_A);
    printf("Array B\n");
    print_arr(B, size_B);
    printf("Array C\n");
    print_arr(C, size_A);

    free(A);
    free(B);
    free(C);
    return 0;
}
