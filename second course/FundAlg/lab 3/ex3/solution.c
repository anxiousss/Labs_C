#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[++i] != '\0');
    return i;
}

int my_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int create_employee(Employee* employee, unsigned int id, char* name, char* surname, double salary) {
    employee->id = id;
    employee->name = (char*)(malloc(sizeof(char) * (len(name) + 1)));
    if (!employee->name) {
        return Memory_leak;
    }
    strcpy(employee->name, name);
    employee->surname = (char*)(malloc(sizeof(char) * (len(surname) + 1)));
    if (!employee->surname) {
        free(employee->name);
        return Memory_leak;
    }
    strcpy(employee->surname, surname);
    employee->salary = salary;
    return 0;
}

void free_employee(Employee** employees, int size) {
    for (int i = 0; i < size; ++i) {
        if (employees[i]) {
            free(employees[i]->name);
            free(employees[i]->surname);
            free(employees[i]);
        }
    }
}

int read_file(Employee*** employees, FILE* file, int* size) {

    unsigned int id;
    char name[100];
    char surname[100];
    double salary;

    int index = 0;
    while (fscanf(file, "%u %s %s %lf", &id, name, surname, &salary) == 4) {
        if (salary < 0) {
            return Invalid_input;
        }

        if (index >= *size) {
            *size *= 2;
            Employee** tmp = (Employee**)realloc(*employees, *size * sizeof(Employee*));
            if (!tmp) {
                return Memory_leak;
            }
            *employees = tmp;
        }

        Employee* employee = (Employee*)(malloc(sizeof(Employee)));
        if (!employee) {
            return Memory_leak;
        }
        int err = create_employee(employee, id, name, surname, salary);
        if (err) {
            free(employee);
            return Memory_leak;
        }

        (*employees)[index] = employee;
        ++index;
    }

    *size = index;
    return 0;
}

int cmpA(const void* emp1, const void* emp2) {
    Employee* a = *(Employee**)emp1;
    Employee* b = *(Employee**)emp2;

    if (a->salary != b->salary)
        return a->salary - b->salary;

    int n = 0;
    n = my_strcmp(a->surname, b->surname);
    if (n)
        return n;

    n = my_strcmp(a->name, b->name);
    if (n)
        return n;

    if (a->id < b->id)
        return -1;

    return 1;
}

void HandlerOptA(Employee** employees, int size) {
    qsort(employees, size, sizeof(Employee*),  cmpA);
}


int cmpD(const void* emp1, const void* emp2) {
    Employee* a = *(Employee**)emp2;
    Employee* b = *(Employee**)emp1;

    if (a->salary != b->salary)
        return a->salary - b->salary;

    int n = 0;
    n = my_strcmp(a->surname, b->surname);
    if (n)
        return n;

    n = my_strcmp(a->name, b->name);
    if (n)
        return n;

    if (a->id < b->id)
        return -1;

    return 1;
}

void HandlerOptD(Employee** employees, int size) {
    qsort(employees, size, sizeof(Employee*), cmpD);
}

void write_employees(Employee** employees, int size, FILE* file) {
    for (int i = 0; i < size; ++i) {
        fprintf(file, "%u %s %s %lf\n", employees[i]->id, employees[i]->name, employees[i]->surname, employees[i]->salary);
    }
}