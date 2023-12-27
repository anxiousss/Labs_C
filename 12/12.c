#include <stdio.h>
#include <math.h>
#pragma warning(disable : 4996)
int main() {
    

    FILE* test;

    if ((test = fopen("test.txt", "r")) == NULL)
    {
        printf("Не удалось открыть файл");
        return 0;
    }
    
    int number, last_digit, number_with_zeros, final_number;

    FILE* result;
    result = fopen("result.txt", "w");
    while (fscanf(test, "%d", &number) != EOF)
    {
        int pos = last_digit = number_with_zeros = final_number = 0;
        while (number > 0)
        {
            last_digit = number % 10;
            number /= 10;
            ++pos;

            if (pos % 2 == 0)
            {
                number_with_zeros = 10 * number_with_zeros + last_digit;
            }

        }
        while (number_with_zeros > 0)
        {
            last_digit = number_with_zeros % 10;
            number_with_zeros /= 10;
            final_number = 10 * final_number + last_digit;
        }

        fprintf(result, "%d", final_number);
        pos = 0;
        last_digit = 0;
        number_with_zeros = 0;
        final_number = 0;
    }

    fclose(test);
    fclose(result);

    return 0;
}