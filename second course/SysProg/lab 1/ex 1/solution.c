#include "solution.h"

int read_line(char **result) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
        if (length + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);

            if (!new_buffer) {
                free(buffer);
                return Memory_leak;
            }

            buffer = new_buffer;
        }

        buffer[length++] = ch;
    }

    buffer[length] = '\0';

    *result = buffer;

    if (ch != '\n' && ch != EOF) {
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }

    return 0;
}

int check_login(const char* login) {
    if (!login) {
        return 0;
    }
    for (int i = 0; login[i]; ++i) {
        if (i > 6 || isalnum(login[i] == 0)) {
            return 0;
        }
    }
    return 1;
}

int check_pin(const int* pin) {
    return (0 <= *pin) && (*pin <= 100000);
}

int init_users(Users** users, size_t capacity, size_t size) {
    (*users) = (Users*)(malloc(sizeof(Users)));
    if (!*users) {
        return Memory_leak;
    }
    (*users)->data = (User*)(malloc(sizeof(User) * capacity));
    if (!(*users)->data) {
        free(*users);
        return Memory_leak;
    }
    (*users)->capacity = capacity;
    (*users)->size = size;
    return 0;
}

int sign_up(Users* users, int* login_index) {
    User user;
    user.login = (char*)malloc(sizeof(char ) * (MAX_LEN + 1));
    if (!user.login) {
        users_destroy(users);
        return Memory_leak;
    }
    printf("Sign up\n");
    printf("Enter a login: \n");
    char* login = NULL;
    int tmp = read_line(&login);
    if (tmp)
        return tmp;
    if (!check_login(login)) {
        printf("Invalid login\n");
        return Wrong_input;
    }
    for (int i = 0; i < users->capacity; ++i) {
        if (strcmp(users->data[i].login, login) == 0) {
            printf("This login is already in use\n");
            return Wrong_input;
        }
    }

    strncpy(user.login, login, strlen(login));
    user.login[strlen(login)] = '\0';
    free(login);
    printf("Enter a pin: \n");
    int pin;
    scanf("%d", &pin);
    if (!check_pin(&pin)) {
        printf("Invalid pin\n");
        return Wrong_input;
    }
    user.pin = pin;
    user.sanctions = -1;
    if (users->capacity == users->size) {
        User* user_tmp = (User*)realloc(users->data, users->capacity * 2 * sizeof(User));
        if (!user_tmp) {
            users_destroy(users);
            return Memory_leak;
        }
        users->capacity *= 2;
        users->data = user_tmp;
    }
    users->data[users->size] = user;
    *login_index = users->size;
    ++users->size;
    return 0;
}

int sign_in(Users* users, int* login_index) {
    printf("Sign in");
    printf("Enter a login: ");
    char* login = NULL;
    int tmp = read_line(&login);
    if (tmp)
        return tmp;
    if (!check_login(login)) {
        printf("Invalid login\n");
        return Wrong_input;
    }
    printf("Enter a pin: \n");
    int pin;
    scanf("%d", &pin);
    if (!check_pin(&pin)) {
        printf("Invalid pin\n");
        return Wrong_input;
    }
    for (int i = 0; i < users->capacity; ++i) {
        if (strcmp(users->data[i].login, login) == 0 && users->data[i].pin == pin) {
            *login_index = i;
            users->data[i].sanctions = -1;
            return 0;
        }
    }
    printf("Wrong login or pass");
    return Wrong_input;
}

int get_time(char* buffer) {
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    strftime(buffer, 9, "%H:%M:%S", time_info);
    return 0;
}

int get_date(char* buffer) {
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    strftime(buffer, 11, "%d:%m:%Y", time_info);
    return 0;
}

int howmuch_time(const char* date, const char* flag, double* diff) {
    struct tm user_date ={0};

    if (sscanf(date, "%d:%d:%d", &user_date.tm_mday, &user_date.tm_mon, &user_date.tm_year) != 3 ||
        (user_date.tm_mon > 12 || user_date.tm_mon < 0) || user_date.tm_year < 0 || user_date.tm_mday < 0 || user_date.tm_mday > 30) {
        return Wrong_input;
    }

    user_date.tm_mon -= 1;
    user_date.tm_year -= 1900;

    time_t user_time = mktime(&user_date);
    if (user_time == -1) {
        return Wrong_input;
    }

    time_t current_time;
    time(&current_time);

    double diff_seconds = difftime(current_time, user_time);
    if (strcmp(flag, "-s") == 0) {
        *diff = diff_seconds;
    } else if (strcmp(flag, "-m") == 0) {
        *diff = diff_seconds / 60;
    } else if (strcmp(flag, "-h") == 0) {
        *diff = diff_seconds / 3600;
    } else if (strcmp(flag, "-y") == 0) {
        *diff = diff_seconds / 31536000.0;
    } else {
        return Wrong_input;
    }
    return 0;
}

int sacntions(Users* users, char* login, int number) {
    int password;
    printf("ENTER A SPECIAL CODE\n");
    scanf("%d", &password);
    if (password == SECRET_PASS) {
        for (int i = 0; i < users->size; ++i) {
            if (strcmp(users->data[i].login, login) == 0) {
                users->data[i].sanctions = number;
                return 0;
            }
        }
        return Wrong_input;
    }
    return Invalid_code;
}


void users_destroy(Users* users) {
    if (!users) return;
    for (int i = 0; i < users->size; ++i) {
        free(users->data[i].login);
    }
    free(users->data);
    free(users);
}

int load(const char* filename, Users* users) {
    if (!filename || !users) {
        return Null_pointer_error;
    }
    FILE* fin = fopen(filename, "rb");
    if (!fin) {
        return File_error;
    }

    User user;
    while (fread(&user, 1, sizeof(user), fin)) {
        if (users->capacity == users->size) {
            User* user_tmp = (User*)realloc(users->data, users->capacity * 2 * sizeof(User));
            if (!user_tmp) {
                users_destroy(users);
                fclose(fin);
                return Memory_leak;
            }
            users->capacity *= 2;
            users->data = user_tmp;
        }
        users->data[users->size] = user;
        users->size++;
    }
    fclose(fin);
    return 0;
}

int save(const char* filename, Users* users) {
    if (!filename || !users) {
        return Null_pointer_error;
    }
    FILE* fin = fopen(filename, "wb");
    if (!fin) {
        return File_error;
    }

    fwrite(users->data, users->size, sizeof(User), fin);
    fclose(fin);
    return 0;
}