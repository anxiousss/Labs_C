#include <stdio.h>
#include <math.h>
#include "hex.h"

void ReverseArray(int arr[], const int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

int* bin(int number, int* len, int* digits) {
    int i = 0;
    while (number != 0) {
        int residue = number % 2;
        number /= 2;
        digits[i] = residue;
        ++i;
    }

    *len = i;
    ReverseArray(digits, i);
    return digits;
}

void hex(int* binary_number, const int* len, int* discharges) {
    int k = 0;
    for (int i = *len - 1; i > -1; i -= 4) {
        int degree = 0;
        int discharge = 0;
        for (int j = i; j != i - 4; --j) {
            discharge += binary_number[j] * (pow(2, degree));
            degree += 1;
        }
        discharges[k] = discharge;
        ++k;
    }
    for (int i = k - 1; i > -1; --i) {
        printf("%X", discharges[i]);
    }
}
