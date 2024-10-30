#include "solution.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Invalid input\n");
        return Invalid_input;
    }

    char in_path[PATH_MAX];
    kOpts opt;
    char out_path[PATH_MAX];

    strcpy(in_path, argv[1]);
    if (argv[2][0] == '-' || argv[2][0] == '/') {
        switch (argv[2][1]) {
            case 'a':
                opt = OPT_A;
                break;
            case 'd':
                opt = OPT_D;
                break;
        }
    } else {
        printf("Invalid Input\n");
        return Invalid_input;
    }
    strcpy(out_path, argv[3]);

    FILE* in = fopen(in_path, "r");
    FILE* out = fopen(out_path, "w");
    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
        printf("Invalid input\n");
        return Invalid_input;
    }

    int size = 10;
    Employee** employees = (Employee**)(malloc(sizeof(Employee*) * size));
    if (!employees) {
        printf("Memory leak\n");
        fclose(in);
        fclose(out);
        return Memory_leak;
    }

    int err = read_file(&employees, in, &size);
    if (err) {
        switch (err) {
            case Invalid_input:
                printf("Invalid input\n");
                break;
            case Memory_leak:
                printf("Memory leak\n");
                break;
        }
        free_employee(employees, size);
        free(employees);
        fclose(out);
        return err;
    }

    void (*handlers[2])(Employee**, int) = {
            HandlerOptA,
            HandlerOptD
    };

    handlers[opt](employees, size);
    write_employees(employees, size, out);

    free_employee(employees, size);
    free(employees);
    fclose(in);
    fclose(out);
    return 0;
}
