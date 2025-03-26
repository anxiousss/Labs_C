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

int handle_choice(char* choice, Users* users, int* flag, const int* n_commands, int* login_index) {
    int err = 0;

    if (*flag == 0) {
        if (strcmp(choice, "SIGN IN") == 0) {
            err = sign_in(users, login_index);
        } else if (strcmp(choice, "SIGN UP") == 0) {
            err = sign_up(users, login_index);
        }
        *flag = 1;
        if (err) {
            return err;
        }
    }
    if (*n_commands <= users->data[*login_index].sanctions || users->data[*login_index].sanctions != -1) {
        if (strcmp(choice, "Time") == 0) {
            char buf[11];
            err = get_time(buf);
            if (err) {
                return err;
            }
            printf("%s\n", buf);
        } else if (strcmp(choice, "Date") == 0) {
            char buf[11];
            err = get_date(buf);
            if (err) {
                return err;
            }
            printf("%s\n", buf);
        } else if (strncmp(choice, "Howmuch", 7) == 0) {
            double diff = 0;
            char time[15];
            char time_flag[3];
            int parsed = sscanf(choice, "Howmuch %14s %2s", time, time_flag);
            if (parsed != 2) {
                return Wrong_input;
            }
            err = howmuch_time(time, time_flag, &diff);
            if (err)
                return err;
            printf("%f", diff);
        } else if (strcmp(choice, "Logout") == 0) {
            *flag = 0;
        } else if (strncmp(choice, "Sanctions", 9) == 0) {
            char login[7];
            int n_sanctions;
            int parsed = sscanf(choice, "Sanctions %6s %d", login, &n_sanctions);
            if (parsed != 2) {
                return Wrong_input;
            }
            err = sacntions(users, login, n_sanctions);
            if (err)
                return err;
        } else if (strcmp(choice, "Exit") == 0) {
            return EXIT;
        } else {
            return Wrong_input;
        }
    }
    return 0;
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
    int is_authorized = 0, n_commands = 0, login_index;
    while (1) {
        char* choice = NULL;
        err = read_line(&choice);
        if (err) {
            users_destroy(users);
            return Memory_leak;
        }
        err = handle_choice(choice, users, &is_authorized, &n_commands, &login_index);
        if (err == EXIT) {
            err = save("database.bin", users);
            if (err)
                return err;
            users_destroy(users);
            return 0;
        } else if (err && err != Memory_leak) {
            users_destroy(users);
            return err;
        }
        if (is_authorized == 0) {
            for (int i = 0; i < users->size; ++i) {
                users->data[i].sanctions = 0;
            }
        } else {
            ++n_commands;
        }
        free(choice);

    }
}