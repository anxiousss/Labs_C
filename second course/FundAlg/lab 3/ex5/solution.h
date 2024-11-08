
#ifndef EX3_5_SOLUTION_H
#define EX3_5_SOLUTION_H

#include "string.h"

typedef struct Student {
    unsigned int id;
    String name;
    String surname;
    String group;
    unsigned int* grades;
} Student;

typedef enum Command {
    HELP = 1,
    SORT_ID,
    SORT_NAME,
    SORT_SURNAME,
    SORT_GROUP,
    FIND_ID,
    FIND_NAME,
    FIND_SURNAME,
    FIND_GROUP,
    OUT_BEST,
    EXIT
}Command;

typedef struct Students {
    Student** students;
    int length;
    int capacity;
} Students;

int check_file_names(const char *file1, const char *file2);

int read_line(FILE* fin, char **result);

int read_id(FILE* fin, unsigned int* id);

int read_name(FILE* fin, String* name);

int read_surname(FILE* fin, String* surname);

int read_group(FILE* fin, String* group);

int read_grades(FILE* fin, unsigned int* grades);

int init_students(Students** students, int length, int capacity);

int delete_student(Student* student);

int delete_students(Students* students);

int read_student(FILE* fin, Student* student);

int read_students(FILE* fin, Students* students);

int resize_students(Students* students, int size);

int add_student(Student* student, Students* students);

int find_id(Students* students, unsigned int id);

int find_name(Students* students, String* name);

int find_surname(Students* students, String* surname);

int find_group(Students* students, String* group);

int cmp_id(const void* a, const void* b);

int cmp_name(const void* a, const void* b);

int cmp_surname(const void* a, const void* b);

int cmp_group(const void* a, const void* b);

void sort_id(Students* students);

void sort_name(Students* students);

void sort_surname(Students* students);

void sort_group(Students* students);

int sum(unsigned int* grades);

void write_student(Student* student, FILE* fin);

void print_menu();

void print_help();

float average_grade(Students* students);

void out_best(Students* students, FILE* fin);

int read_choice(int* choice);

int handle_choice(int choice, FILE* fin, Students* students);

int dialog_manager(FILE* in, FILE* out);

#endif //EX3_5_SOLUTION_H
