#include "solution.h"

void print_menu() {
    printf("1  - HELP\n");
    printf("2  - SORT_ID\n");
    printf("3  - SORT_NAME\n");
    printf("4  - SORT_SURNAME\n");
    printf("5  - SORT_GROUP\n");
    printf("6  - FIND_ID\n");
    printf("7  - FIND_NAME\n");
    printf("8  - FIND_SURNAME\n");
    printf("9  - FIND_GROUP\n");
    printf("10 - OUT_BEST\n");
    printf("11 - OUT_STUDENTS\n");
    printf("12 - EXIT\n");
}

void print_help() {
    printf("2/SORT_ID - Sort by id.\n");
    printf("3/SORT_NAME - Sort by name.\n");
    printf("4/SORT_SURNAME - Sort by surname.\n");
    printf("5/SORT_GROUP - Sort by group.\n");
    printf("6/FIND_ID - Find by id.\n");
    printf("7/FIND_NAME - Find by name.\n");
    printf("8/FIND_SURNAME - Find by surname.\n");
    printf("9/FIND_GROUP - Find by group.\n");
    printf("10/OUT_BEST - Output best students in file.\n");
    printf("11/OUT_STUDENTS - Output all students in file\n");
    printf("11/EXIT\n");
}

void write_student(Student* student, FILE* fin) {
    fprintf(fin, "Id: %u Name: %s Surname: %s Group: %s Average: %lf\n", student->id, student->name.mas, student->surname.mas, student->group.mas,
            average_student(student));
}

void out_best(Students* students, FILE* fin) {
    double average = average_grade(students);
    for (int i = 0; i < students->length; ++i) {
        if (average_student(students->students[i]) > average) {
            write_student(students->students[i], fin);
        }
    }
}

int read_choice(int* choice) {
    char c;
    scanf("%d%c", choice, &c);
    if (*choice < 1 || *choice > 12)
        return Invalid_input;
    return 0;
}

void out_students(Students* students, FILE* fin) {
    for (int i = 0; i < students->length; ++i) {
        write_student(students->students[i], fin);
    }
}

int handle_choice(int choice, FILE* fin, Students* students) {
    unsigned int id;
    String name, surname, group;
    int index;
    int err;
    switch (choice) {
        case HELP:
            print_help();
            return 0;

        case SORT_ID:
            sort_id(students);
            return 0;

        case SORT_NAME:
            sort_name(students);
            return 0;

        case SORT_SURNAME:
            sort_surname(students);
            return 0;

        case SORT_GROUP:
            sort_group(students);
            return 0;

        case FIND_ID:
            err = read_id(stdin, &id);
            if (err)
                return err;
            index = find_id(students, id);
            if (index == -1) {
                printf("NOT FOUND\n");
                return 0;
            }
            printf("FOUND\n");
            write_student(students->students[index], fin);
            return 0;

        case FIND_NAME:
            err = read_name(stdin, &name);
            if (err)
                return err;
            index = find_name(students, &name);
            if (index == -1) {
                printf("NOT FOUND\n");
                return 0;
            }
            printf("FOUND\n");
            write_student(students->students[index], fin);
            return 0;

        case FIND_SURNAME:
            err = read_surname(stdin, &surname);
            if (err)
                return err;
            index = find_surname(students, &surname);
            if (index == -1) {
                printf("NOT FOUND\n");
                return 0;
            }
            printf("FOUND\n");
            write_student(students->students[index], fin);
            return 0;

        case FIND_GROUP:
            err = read_group(stdin, &group);
            if (err)
                return err;
            index = find_group(students, &group);
            if (index == -1) {
                printf("NOT FOUND\n");
                return 0;
            }
            printf("FOUND\n");
            write_student(students->students[index], fin);
            return 0;

        case OUT_BEST:
            out_best(students, fin);
            return 0;

        case OUT_STUDENTS:
            out_students(students, fin);
            return 0;

        default:
            return Invalid_input;
    }
}



int dialog_manager(FILE* in, FILE* out) {
    Students* students;
    int err = init_students(&students, 0, 2);
    if (err)
        return err;
    err = read_students(in, students);
    if (err) {
        delete_students(students);
        return err;
    }
    while (1) {
        print_menu();
        int choice;
        err = read_choice(&choice);
        if (err) {
            delete_students(students);
            return err;
            }
        if (choice == EXIT) {
            delete_students(students);
            return 0;
        }
        err = handle_choice(choice, out, students);
        if (err) {
            delete_students(students);
            return err;
        }
    }
}