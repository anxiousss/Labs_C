#include <stdio.h>

#define SIZE 4

void printMatrixClockwise(int matrix[SIZE][SIZE]) {
    int rowStart = 0;
    int rowEnd = SIZE - 1;
    int colStart = 0;
    int colEnd = SIZE - 1;
    int i;

    while (rowStart <= rowEnd && colStart <= colEnd) {
        // Вывод элементов верхней строки
        for (i = colStart; i <= colEnd; i++) {
            printf("%d ", matrix[rowStart][i]);
        }
        rowStart++;

        // Вывод элементов последнего столбца
        for (i = rowStart; i <= rowEnd; i++) {
            printf("%d ", matrix[i][colEnd]);
        }
        colEnd--;

        // Вывод элементов нижней строки в обратном порядке
        if (rowStart <= rowEnd) {
            for (i = colEnd; i >= colStart; i--) {
                printf("%d ", matrix[rowEnd][i]);
            }
            rowEnd--;
        }

        // Вывод элементов первого столбца в обратном порядке
        if (colStart <= colEnd) {
            for (i = rowEnd; i >= rowStart; i--) {
                printf("%d ", matrix[i][colStart]);
            }
            colStart++;
        }
    }
}

int main() {
    int matrix[SIZE][SIZE] = {
        {4, 5, 6, 7},
        {3, 14, 15, 8},
        {2, 13, 16, 9},
        {1, 12, 11, 10}
    };

    printMatrixClockwise(matrix);

    return 0;
}