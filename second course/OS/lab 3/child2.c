#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define SHARED_MEM_SIZE 4096

#define SHARED_MEM_NAME2 "/shared_memory2"

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
        write(STDERR_FILENO, "Usage: child2 <filename>\n", 25);
        exit(EXIT_FAILURE);
    }

    int memory = shm_open(SHARED_MEM_NAME2, O_RDWR, 0666);
    if (memory == -1) {
        exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1) {
        shm_unlink(SHARED_MEM_NAME2);
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_LINE_LENGTH];
    ssize_t len;
    while ((len = read(memory, buffer, MAX_LINE_LENGTH)) > 0) {
        if (strcmp(buffer, "exit") == 0) {
            close(file);
            shm_unlink(SHARED_MEM_NAME2);
            printf("Child 2 Exit\n");
            break;
        }
        reverse_string(buffer);
        write(file, buffer, len);
        write(file, "\n", 1);
    }

    return 0;
}
