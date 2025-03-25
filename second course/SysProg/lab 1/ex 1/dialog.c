#include "solution.h"

void print_menu() {
    printf("1 - SIGN IN\n");
    printf("2 - SIGN UP\n");
    printf("3 - TIME\n");
    printf("4 - DATE\n");
    printf("5 - HOWMUCH TIME\n");
    printf("6 - LOGOUT\n");
    printf("7 - SANCTIONS\n");
}

int handle_choice(char* choice, Users* users) {
    int err = 0;
    if (strcmp(choice, "SIGN IN") == 0) {
        err = sign_in(users);
    } else if (strcmp(choice, "SIGN UP") == 0) {
        err = sign_up(users);
    } else if (strcmp(choice, "Time") == 0) {
        char buf[11];
        err = get_time(buf);
        if (err) {
            return err;
        }
        printf("%s\n", buf);
    }  else if(strcmp(choice, "Date") == 0) {
        char buf[11];
        err = get_date(buf);
        if (err) {
            return err;
        }
        printf("%s\n", buf);
    } else if (strcmp(choice, "How much "))


    if (err) {
        return err;
    }
}

int dialog_manager() {
    Users* users;
    int err = init_users(&users, 2, 0);
    if (err) {
        return err;
    }
    err = load("database.bin", users);
    if (err)
        return err;
    print_menu();
    while (1) {
        char* choice = NULL;
        read_line(&choice);
        err = handle_choice(choice, users);
        if (err)
            return err;
    }
    return 1;
}