#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define MAX_LINE_LENGTH 1024

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2;
    char filename1[MAX_LINE_LENGTH], filename2[MAX_LINE_LENGTH];
    char buffer[MAX_LINE_LENGTH];
    int line_number = 1;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Enter filename for child1: ", 27);
    ssize_t len = read(STDIN_FILENO, filename1, MAX_LINE_LENGTH);
    filename1[len - 1] = '\0';

    write(STDOUT_FILENO, "Enter filename for child2: ", 27);
    len = read(STDIN_FILENO, filename2, MAX_LINE_LENGTH);
    filename2[len - 1] = '\0'; 

    pid1 = fork();
    if (pid1 == -1) {
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        close(pipe1[1]); 
        dup2(pipe1[0], STDIN_FILENO); 
        execl("./child1", "child1", filename1, NULL);

        close(pipe1[0]); 

        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1) {
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        close(pipe2[1]);
        dup2(pipe2[0], STDIN_FILENO); 
        execl("./child2", "child2", filename2, NULL);
        close(pipe2[0]);

        exit(EXIT_FAILURE);
    }

    // Родительский процесс
    close(pipe1[0]);
    close(pipe2[0]);

    while (1) {
        write(STDOUT_FILENO, "Enter a line (or 'exit' to quit): ", 34);
        len = read(STDIN_FILENO, buffer, MAX_LINE_LENGTH);
        buffer[len - 1] = '\0'; 

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        if (line_number % 2 == 1) {
            write(pipe1[1], buffer, strlen(buffer) + 1);
        } else {
            write(pipe2[1], buffer, strlen(buffer) + 1);
        }

        line_number++;
    }

    shutdown(pipe1[1], SHUT_WR);
    shutdown(pipe2[1], SHUT_WR); 

    wait(NULL);
    wait(NULL);

    return 0;
}
