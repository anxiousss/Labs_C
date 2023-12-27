#include <stdio.h>

#define SIZE 4

int findMaxAndMinRowSum(int matrix[SIZE][SIZE], int n) {
    int i, j;
    int maxElement = matrix[0][0];
    int minRow = 0;

    // Находим максимальный элемент и строку с минимальным номером
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(matrix[i][j] > maxElement) {
                maxElement = matrix[i][j];
                minRow = i;
            }
        }
    }

    // Находим сумму элементов в найденной строке
    int sum = 0;
    for(j = 0; j < n; j++) {
        sum += matrix[minRow][j];
    }

    return sum;
}

int main() {
    int matrix[SIZE][SIZE] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    int sum = findMaxAndMinRowSum(matrix, SIZE);

    printf("Сумма элементов строки с минимальным номером, содержащей максимальный элемент: %d\n", sum);

    return 0;
}