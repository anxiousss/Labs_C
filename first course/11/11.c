#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable : 4996)

int main() {

    const int max_number = pow(2, 16) - 1;

    FILE* file;
    if ((file = fopen("test.txt", "r")) == NULL) {
        printf("Не удалось открыть файл");
        return 0;
    }

    int n_numbers = 0, ch, number = 0;

    while ((ch = getc(file)) != EOF) {

        if ('0' <= ch && ch <= '9') {
            if (number < max_number)
                number = number * 10 + ch - '0';

        }
        else {
            if (0 < number && number <= max_number)
                n_numbers++;

            number = 0;
        }
    }

    if (0 < number && number <= max_number)
        n_numbers++;

    fclose(file);

    file = fopen("result.txt", "w");

    fprintf(file, "%d", n_numbers);

    fclose(file);

    return 0;
}
