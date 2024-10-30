

#ifndef EX3_3_SOLUTION_H
#define EX3_3_SOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

typedef enum kOpts {
    OPT_A = 0,
    OPT_D
} kOpts;

typedef struct Employee {
    unsigned int id;
    char* name;
    char* surname;
    double salary;
} Employee;

int len(const char* str);

int my_strcmp(const char* str1, const char* str2);

int create_employee(Employee* employee, unsigned int id, char* name, char* surname, double salary);

void free_employee(Employee** employees, int size);

int read_file(Employee*** employees, FILE* file, int* size);

int cmpA(const void* emp1, const void* emp2);

void HandlerOptA(Employee** employees, int size);

int cmpD(const void* emp1, const void* emp2);

void HandlerOptD(Employee** employees, int size );

void write_employees(Employee** employees, int size, FILE* file);

#endif //EX3_3_SOLUTION_H
