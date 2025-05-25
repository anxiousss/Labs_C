#include "client.hpp"
#include <iostream>

void interface() {
    std::cout << "\n=== Command List ===\n"
              << "1 <file.cpp> - Compile C++ file\n"
              << "2 <file.tex> - Compile LaTeX\n"
              << "3 <number>   - Take sticks (1-3)\n"
              << "exit         - Quit\n"
              << "========================\n";
}

bool check_msg(const std::string& msg) {
    if(msg.empty()) return false;

    switch(msg[0]) {
        case '1':
            return msg.size() > 2 &&
                   (msg.substr(2).ends_with(".cpp") ||
                    msg.substr(2).ends_with(".CPP"));

        case '2':
            return msg.size() > 2 &&
                   (msg.substr(2).ends_with(".tex") ||
                    msg.substr(2).ends_with(".TEX"));

        case '3': {
            int num = std::stoi(msg.substr(2));
            return num >= 1 && num <= 3;
        }

        case 'e':
            return msg == "exit";

        default:
            return false;
    }
}