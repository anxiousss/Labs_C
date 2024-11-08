#include "solution.h"

int check_file_names(const char *file1, const char *file2) {
    const char * name1 = strrchr(file1,'\\');
    const char * name2 = strrchr(file2,'\\');
    if (name1 != NULL) {
        name1++;
    } else {
        name1 = file1;
    }
    if (name2 != NULL) {
        name2++;
    } else {
        name2 = file2;
    }

    if (strcmp(name1, name2) == 0) {
        return Equal_Paths;
    }
    return 0;
}

int init_students(Students** students, int length, int capacity) {
    *students = (Students*)(malloc(sizeof(Students)));
    if (!(*students))
        return Memory_leak;

    (*students)->length = length;
    (*students)->capacity = capacity;

    (*students)->students = (Student**)(malloc(sizeof(Student*) * capacity));
    if (!(*students)->students) {
        free(*students);
        return Memory_leak;
    }
    return 0;
}
int read_line(FILE* fin, char **result) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(fin)) != ' ' && ch != EOF) {
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

    return 0;
}
int read_id(FILE* fin, unsigned int* id) {
    char c;
    int n = fscanf(fin, "%u%c", id, &c);
    if (n != 2)
        return Invalid_input;
    return 0;
}

int read_name(FILE* fin, String* name) {
    char* tmp = NULL;
    int err = 0;
    err = read_line(fin, &tmp);
    if (err == 0) {
        err = init_string(name, tmp);
    }
    return err;
}

int read_surname(FILE* fin, String* surname) {
    char* tmp = NULL;
    int err = 0;
    err = read_line(fin, &tmp);
    if (err == 0) {
        err = init_string(surname, tmp);
    }
    return err;
}

int read_group(FILE* fin, String* group) {
    char* tmp = NULL;
    int err = 0;
    err = read_line(fin, &tmp);
    if (err == 0) {
        err = init_string(group, tmp);
    }
    return err;
}

int read_grades(FILE* fin, unsigned int** grades) {
    *grades = (unsigned int* )(malloc(sizeof(unsigned int) * 5));
    if (!(*grades))
        return Memory_leak;
    int c;
    int n = fscanf(fin, "%u %u %u %u %u", *grades, *grades + 1, *grades + 2, *grades+  3, *grades + 4);
    c = fgetc(fin);
    if (n != 5)
        return Invalid_input;
    return 0;
}

int read_student(FILE* fin, Student* student) {
    int err = 0;
    err = read_id(fin, &(student->id));
    if (err == 0)
        err = read_name(fin, &(student->name));

    if (err == 0)
        err = read_surname(fin, &(student->surname));

    if (err == 0)
        err = read_group(fin, &(student->group));

    if (err == 0)
        err = read_grades(fin, &(student->grades));

    return err;
}

int read_students(FILE* fin, Students* students) {
    while (!feof(fin)) {
        Student* student = (Student*)(malloc(sizeof(Student)));
        if (!student) {
            delete_students(students);
            return Memory_leak;
        }
        int err = read_student(fin, student);
        if (err)
            return err;
        err = add_student(student, students);
        if (err)
            return err;
    }
    return 0;
}

int delete_student(Student* student) {
    delete_string(&student->name);
    delete_string(&student->surname);
    delete_string(&student->group);
    free(student->grades);
    return 0;
}

int delete_students(Students* students) {
    for (int i = 0; i < students->length; ++i) {
        delete_student(students->students[i]);
    }
    free(students);
    return 0;
}

int resize_students(Students* students, int size) {
    Student*** pStudent = &(students->students);
    Student** tmp = (Student**)(realloc(*pStudent, size * sizeof(Student*)));
    if (!tmp) {
        delete_students(students);
        return Memory_leak;
    }
    *pStudent = tmp;
    return 0;
}

int add_student(Student* student, Students* students) {
    if (students->capacity == students->length) {
        students->capacity *= 2;
        int err = resize_students(students, students->capacity);
        if (err)
            return err;
    }
    students->students[students->length] = student;
    ++(students->length);
    return 0;
}

int find_id(Students* students, unsigned int id) {
    for (int i = 0; i < students->length; ++i) {
        if (students->students[i]->id == id) {
            return i;
        }
    }
    return -1;
}

int find_name(Students* students, String* name) {
    for (int i = 0; i < students->length; ++i) {
        if (eq_string(&students->students[i]->name, name)) {
            return i;
        }
    }
    return -1;
}

int find_surname(Students* students, String* surname) {
    for (int i = 0; i < students->length; ++i) {
        if (eq_string(&students->students[i]->surname, surname)) {
            return i;
        }
    }
    return -1;
}

int find_group(Students* students, String* group) {
    for (int i = 0; i < students->length; ++i) {
        if (eq_string(&students->students[i]->group, group)) {
            return i;
        }
    }
    return -1;
}

int cmp_id(const void* a, const void* b) {
    Student* first = *(Student**)a;
    Student* second = *(Student**)b;
    return first->id - second->id;
}
int cmp_name(const void* a, const void* b) {
    Student* first = *(Student**)a;
    Student* second = *(Student**)b;
    return cmp_string(&first->name, &second->name);
}

int cmp_surname(const void* a, const void* b) {
    Student* first = *(Student**)a;
    Student* second = *(Student**)b;
    return cmp_string(&first->surname, &second->surname);
}

int cmp_group(const void* a, const void* b) {
    Student* first = *(Student**)a;
    Student* second = *(Student**)b;
    return cmp_string(&first->group, &second->group);
}

void sort_id(Students* students) {
    qsort(students->students, students->length, sizeof(Student*), cmp_id);
}

void sort_name(Students* students) {
    qsort(students->students, students->length, sizeof(Student*), cmp_name);

}

void sort_surname(Students* students) {
    qsort(students->students, students->length, sizeof(Student*), cmp_surname);

}

void sort_group(Students* students) {
    qsort(students->students, students->length, sizeof(Student*), cmp_group);

}

double average_student(Student* student) {
    double total = 0.0;
    for (int i = 0; i < 5; ++i) {
        total += (double )student->grades[i];
    }
    return total / 5.0;
}

double average_grade(Students* students) {
    double average = 0;
    for (int i = 0; i < students->length; ++i) {
        average += average_student(students->students[i]);
    }
    average /= (double )students->length;
    return average;
}
