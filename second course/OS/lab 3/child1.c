#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>


#define MAX_LINE_LENGTH 1024
#define SHARED_MEM_SIZE 4096

#define SHARED_MEM_NAME1 "/shared_memory1"


void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(STDERR_FILENO, "Usage: child1 <filename>\n", 25);
        exit(EXIT_FAILURE);
    }

    int memory = shm_open(SHARED_MEM_NAME1, O_RDWR, 0666);
    if (memory == -1) {
        exit(EXIT_FAILURE);
    }
    char* shared_memory = mmap(NULL, SHARED_MEM_SIZE, PROT_READ, MAP_SHARED, memory, 0);
    if (shared_memory == MAP_FAILED) {
        shm_unlink(SHARED_MEM_NAME1);
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    printf("ROFLS");
    if (file == -1) {
        munmap(shared_memory, SHARED_MEM_SIZE);
        shm_unlink(SHARED_MEM_NAME1);
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_LINE_LENGTH];
    char* str = shared_memory;
    while (str != NULL) {
        int i = 0;
        while (*str != '\n') {
            buffer[i++] = *str;
            ++str;
        }
        reverse_string(buffer);
        if (strcmp(buffer, "exit") == 0) {
            munmap(shared_memory, SHARED_MEM_SIZE);
            shm_unlink(SHARED_MEM_NAME1);
            close(file);
            printf("Child 1 exit\n");
            exit(EXIT_SUCCESS);
        }
        write(file, buffer, sizeof(buffer));
        write(file, "\n", 1);
    }
    return 0;
}
