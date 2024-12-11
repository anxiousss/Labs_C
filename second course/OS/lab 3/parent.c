#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define SHARED_MEM_SIZE 4096
#define SHARED_MEM_NAME1 "/shared_memory1"
#define SHARED_MEM_NAME2 "/shared_memory2"


int main(void) {
    char filename1[MAX_LINE_LENGTH], filename2[MAX_LINE_LENGTH];
    char buffer[MAX_LINE_LENGTH] = {0};
    int line_number = 1;

    pid_t pid1, pid2;

    write(STDOUT_FILENO, "Enter filename for child1: ", 27);
    ssize_t len = read(STDIN_FILENO, filename1, MAX_LINE_LENGTH);
    filename1[len - 1] = '\0';

    write(STDOUT_FILENO, "Enter filename for child2: ", 27);
    len = read(STDIN_FILENO, filename2, MAX_LINE_LENGTH);
    filename2[len - 1] = '\0';



    int shm_fd1 = shm_open(SHARED_MEM_NAME1,  O_RDWR | O_CREAT, 0666);
    if (shm_fd1 == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    int shm_fd2 = shm_open(SHARED_MEM_NAME2, O_RDWR | O_CREAT, 0666);
    if (shm_fd2 == -1) {
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd1, SHARED_MEM_SIZE) == -1) {
        shm_unlink(SHARED_MEM_NAME1);
        shm_unlink(SHARED_MEM_NAME2);
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd2, SHARED_MEM_SIZE) == -1) {
        shm_unlink(SHARED_MEM_NAME1);
        shm_unlink(SHARED_MEM_NAME2);
        exit(EXIT_FAILURE);
    }

    char* shared_mem1 = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
    if (shared_mem1 == MAP_FAILED) {
        exit(EXIT_FAILURE);
    }
    char* shared_mem2 = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
    if (shared_mem2 == MAP_FAILED) {
        munmap(shared_mem1, SHARED_MEM_SIZE);
        exit(EXIT_FAILURE);
    }

    while (1) {
        write(STDOUT_FILENO, "Enter a line (or 'exit' to quit): ", 34);
        len = read(STDIN_FILENO, buffer, MAX_LINE_LENGTH);
        buffer[len - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            printf("Exit\n");
            snprintf(shared_mem1, SHARED_MEM_SIZE, "%s", "exit");
            snprintf(shared_mem2, SHARED_MEM_SIZE, "%s", "exit");
            break;
        }

        if (line_number % 2 == 1) {
            snprintf(shared_mem1, SHARED_MEM_SIZE, "%s", buffer);
        } else {
            snprintf(shared_mem2, SHARED_MEM_SIZE, "%s", buffer);
        }

        memset(buffer, 0, MAX_LINE_LENGTH);
        line_number++;
    }

    pid1 = fork();
    if (pid1 == -1) {
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        execl("./child1", "child1", filename1, NULL);
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1) {
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        execl("./child2", "child2", filename2, NULL);
        exit(EXIT_FAILURE);
    }

    munmap(shared_mem1, SHARED_MEM_SIZE);
    munmap(shared_mem2, SHARED_MEM_SIZE);
    shm_unlink(SHARED_MEM_NAME1);
    shm_unlink(SHARED_MEM_NAME2);
    printf("WTF\n");
    wait(NULL);
    wait(NULL);
    return 0;
}
