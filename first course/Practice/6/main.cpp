#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Person {
    std::string surname;
    std::string initials;
    int school_number;
    bool medal_have;
    int scores;
    bool credit;
};

std::vector<Person> readPeopleFromFile(const std::string& filename) {
    std::vector<Person> people;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return people;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Person person;

        if (iss >> person.surname >> person.initials >> person.school_number >> person.medal_have >> person.scores >> person.credit) {
            people.push_back(person);
        } else {
            std::cerr << "Error reading person data from line: " << line << std::endl;
        }
    }

    file.close();
    return people;
}

int main() {
    std::string filename = "test.txt";
    std::vector<Person> people = readPeopleFromFile(filename);

    for (const auto& person : people) {
        if (person.medal_have && person.scores <= 40) {
            std::cout << "Surname: " << person.surname << ", Initials: " << person.initials
                      << ", School Number: " << person.school_number << ", Medal: "
                      << (person.medal_have ? "Yes" : "No")
                      << ", Scores: " << person.scores << ", Credit: " << (person.credit ? "Yes" : "No") << std::endl;
        }
    }

    return 0;
}