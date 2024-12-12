#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024
#define SHARED_MEM_SIZE (MAX_LINE_LENGTH + 1)
#define SHARED_MEM_NAME1 "/shared_memory1"
#define SHARED_MEM_NAME2 "/shared_memory2"
#define SEM_NAME1 "/sem1"
#define SEM_NAME2 "/sem2"

int main() {
    char filename1[MAX_LINE_LENGTH], filename2[MAX_LINE_LENGTH];
    char buffer[MAX_LINE_LENGTH];
    int line_number = 1;

    pid_t pid1, pid2;

    write(STDOUT_FILENO, "Enter filename for child1: ", 27);
    ssize_t bytes_read = read(STDIN_FILENO, filename1, sizeof(filename1) - 1);
    if (bytes_read <= 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    filename1[bytes_read - 1] = '\0';

    write(STDOUT_FILENO, "Enter filename for child2: ", 27);
    bytes_read = read(STDIN_FILENO, filename2, sizeof(filename2) - 1);
    if (bytes_read <= 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    filename2[bytes_read - 1] = '\0';

    int shm_fd1 = shm_open(SHARED_MEM_NAME1, O_CREAT | O_RDWR, 0666);
    if (shm_fd1 == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd1, SHARED_MEM_SIZE) == -1) {
        shm_unlink(SHARED_MEM_NAME1);
        close(shm_fd1);
        exit(EXIT_FAILURE);
    }
    char *shared_mem1 = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
    if (shared_mem1 == MAP_FAILED) {
        shm_unlink(SHARED_MEM_NAME1);
        close(shm_fd1);
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *sem1 = sem_open(SEM_NAME1, O_CREAT, 0666, 0);
    if (sem1 == SEM_FAILED) {
        munmap(shared_mem1, MAX_LINE_LENGTH);
        shm_unlink(SHARED_MEM_NAME1);
        close(shm_fd1);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    int shm_fd2 = shm_open(SHARED_MEM_NAME2, O_CREAT | O_RDWR, 0666);
    if (shm_fd2 == -1) {
        sem_close(sem1);
        sem_unlink(SEM_NAME1);
        munmap(shared_mem1, MAX_LINE_LENGTH);
        shm_unlink(SHARED_MEM_NAME1);
        close(shm_fd1);
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd2, SHARED_MEM_SIZE) == - 1) {
        sem_close(sem1);
        sem_unlink(SEM_NAME1);
        munmap(shared_mem1, MAX_LINE_LENGTH);
        shm_unlink(SHARED_MEM_NAME1);
        shm_unlink(SHARED_MEM_NAME2);
        close(shm_fd1);
        close(shm_fd2);
        exit(EXIT_FAILURE);
    }
    char *shared_mem2 = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
    if (shared_mem2 == MAP_FAILED) {
        sem_close(sem1);
        sem_unlink(SEM_NAME1);
        munmap(shared_mem1, MAX_LINE_LENGTH);
        shm_unlink(SHARED_MEM_NAME1);
        shm_unlink(SHARED_MEM_NAME2);
        close(shm_fd1);
        close(shm_fd2);
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *sem2 = sem_open(SEM_NAME2, O_CREAT, 0666, 0);
    if (sem2 == SEM_FAILED) {
        sem_close(sem1);
        sem_unlink(SEM_NAME1);
        munmap(shared_mem1, MAX_LINE_LENGTH);
        munmap(shared_mem2, MAX_LINE_LENGTH);
        shm_unlink(SHARED_MEM_NAME1);
        shm_unlink(SHARED_MEM_NAME2);
        close(shm_fd1);
        close(shm_fd2);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Fork child1
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) {
        execl("./child1", "child1", filename1, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Fork child2
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {
        execl("./child2", "child2", filename2, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    while (1) {
        write(STDOUT_FILENO, "Enter a line (or 'exit' to quit): ", 34);
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            perror("read");
            break;
        }
        buffer[bytes_read - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            strcpy(shared_mem1, "exit");
            sem_post(sem1);
            strcpy(shared_mem2, "exit");
            sem_post(sem2);
            break;
        }

        if (line_number % 2 == 1) {
            strcpy(shared_mem1, buffer);
            sem_post(sem1);
        } else {
            strcpy(shared_mem2, buffer);
            sem_post(sem2);
        }

        line_number++;
    }


    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    munmap(shared_mem1, SHARED_MEM_SIZE);
    shm_unlink(SHARED_MEM_NAME1);
    sem_close(sem1);
    sem_unlink(SEM_NAME1);

    munmap(shared_mem2, SHARED_MEM_SIZE);
    shm_unlink(SHARED_MEM_NAME2);
    sem_close(sem2);
    sem_unlink(SEM_NAME2);

    return 0;
}