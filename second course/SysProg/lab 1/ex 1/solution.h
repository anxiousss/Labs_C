#ifndef SYS_PROG_SOLUTION_H
#define SYS_PROG_SOLUTION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LEN 6
#define SECRET_PASS 12345

typedef enum Errors {
    Wrong_input = 2,
    Memory_leak,
    Invalid_code,
    File_error
} Errors;

typedef enum Command {
    SIGN_IN = 1,
    SIGN_UP,
    TIME,
    DATE,
    HOWMUCH_TIME,
    LOGOUT,
    SANCTIONS,
    UNKNOWN
} Command;

typedef struct User {
    char* login;
    int pin;
    int sanctions;
} User;

typedef struct Users {
    User* data;
    size_t capacity;
    size_t size;
} Users;

int read_choice(int* choice);

int read_line(char** result);

int load(const char* filename, Users* users);

int save(const char* filename, Users* users);

int init_users(Users** users, size_t capacity, size_t size);

int check_login(const char* login);

int check_pin(const int* pin);

int sign_up(Users* users);

int sign_in(Users* users);

int get_time(char* buffer);

int get_date(char* buffer);

int howmuch_time(const char* date, const char* flag, double* diff);

int sacntions(Users* users, char* login, int number);

void users_destroy(Users* users);

void print_menu();

int handle_choice(char* choice, Users* users);

int dialog_manager();


#endif //SYS_PROG_SOLUTION_H
