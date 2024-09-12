#include <iostream>
#include <cstring>

struct Person {
    char surname[20];
    char initials[20];
    int school_number;
    int medal_have;
    int scores;
    int credit;
};

int read_person(Person* p) {
    return scanf("%s %s %d %d %d %d", p->surname, p->initials, &p->school_number, &p->medal_have, &p->scores, &p->credit) == 6;
}


void to_bin(const char* filename) {
    FILE* f = std::fopen(filename, "wb");
    Person person;
    while (read_person(&person)) {
        fwrite(&person, sizeof(person), 1, f);
    }
    fclose(f);
}

void print(const char* filename) {
    FILE* f = std::fopen(filename, "rb");
    if (!f) {
        perror("Error file");
    }
    Person p;
    while (fread(&p, sizeof(p), 1, f) == 1) {
        std::cout << p.surname << ' ' << p.initials << ' ' << p.school_number << ' ' << (p.medal_have ? "Yes": "No")
                  << ' ' << p.scores << ' ' << (p.credit ? "Yes": "No") << std::endl;
    }

    fclose(f);
}

void sol(const char* filename) {
    FILE* f = std::fopen(filename, "rb");

    if (!f) {
        perror("Error file");
    }
    Person p;
    while (fread(&p, sizeof(p), 1, f)) {
        if (p.medal_have && p.scores <= 2) {
            std::cout << p.surname << ' ' << p.initials << ' ' << p.school_number << ' ' << (p.medal_have ? "Yes": "No")
                      << ' ' << p.scores << ' ' << (p.credit ? "Yes": "No") << std::endl;
        }
    }
}



int main(int argc, char* argv[]) {

    if (argc == 3) {
        if (strcmp(argv[1], "-d") == 0) {
            to_bin(argv[2]);
        } else if (strcmp(argv[1], "-f") == 0) {
            print(argv[2]);
        } else if(strcmp(argv[1], "-p") == 0) {
            sol(argv[2]);
        }
    }
    return 0;
}
