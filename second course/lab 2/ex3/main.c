#include "solution.h"


int main() {
    char** result = (char**)(malloc(sizeof(char* ) * 10));
    for (int i = 0; i < 10; ++i) {
        result[i] = (char*)(malloc(sizeof(char ) * 10));
    }
    const char* sub = "da";
    int reps;
    int res = find_sub_files( &result, sub, &reps, "1.txt", "2.txt", NULL);
    if (res) {
        printf("Memory leak");
        return res;
    }
    for (int i = 0; i < reps; ++i) {
        printf("%s\n", result[i]);
    }
    free(result);
    return 0;
}