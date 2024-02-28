#include <stdio.h>
#include <stdlib.h>

const int SIZE = 4;  

void printMatrixClockwise(int** matrix) {
    int rowStart = 0;
    int rowEnd = SIZE - 1;
    int colStart = 0;
    int colEnd = SIZE - 1;
    int i;

    while (rowStart <= rowEnd && colStart <= colEnd) {
        // Вывод элементов верхней строки
        for (i = colEnd; i >= colStart; --i) {
            printf("%d ", *(*(matrix + rowStart) + i));
        }
        ++rowStart;

        // Вывод элементов последнего столбца
        for (i = rowStart; i <= rowEnd; ++i) {
            printf("%d ", *(*(matrix + i) + colStart));
        }
        ++colStart;

        // Вывод элементов нижней строки в обратном порядке
        if (rowStart <= rowEnd) {
            for (i = colStart; i <= colEnd; ++i) {
                printf("%d ", *(*(matrix + rowEnd) + i));
            }
            --rowEnd;
        }

        // Вывод элементов первого столбца в обратном порядке
        if (colStart <= colEnd) {
            for (i = rowEnd; i >= rowStart; --i) {
                printf("%d ", *(*(matrix + i) + colEnd));
            }
            --colEnd;
        }
    }
}

int main() {
    int** matrix = (int**)(malloc(sizeof(int*) * SIZE));

    for (int i = 0; i < SIZE; ++i) {
        *(matrix + i) = (int*)(malloc(sizeof(int) * SIZE));

        for (int j = 0; j < SIZE; ++j) {
            scanf("%d", (*(matrix + i)) + j);
        }
    }

    printMatrixClockwise(matrix);

    for (int i = 0; i < SIZE; ++i) {
        free(*(matrix + i));
    }

    free(matrix);   
    return 0;
}
