#include "solution.h"

int main() {
    int total_results;
    char** results = NULL;

    int error = FindFiles("s\r\nd\r\nr", 2, &results, &total_results, "1.txt", "2.txt");

    switch(error) {
        case Success:
            for (int i = 0; i < total_results; i++) {
                printf("%s", results[i]);
                free(results[i]);
            }
            free(results);
            break;
        case Invalid_input:
            printf("Invalid input\n");
            break;
        case Memory_allocation_error:
            printf("Memory allocation failed\n");
            break;
        case File_open_error:
            printf("File open error\n");
            break;
        default:
            printf("Unknown error\n");
            break;
    }

    return 0;
}