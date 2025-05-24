#include "client.hpp"

void interface() {
    std::cout << "Choose one of three options." << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "1 <file_name> - Compile cpp to exe." << std::endl;
    std::cout << "2 <file_name> - Compile tex to pdf." << std::endl;
    std::cout << "3 <n_sticks to be taken> - Play stick game." << std::endl;
    std::cout << "exit to exit" << std::endl;
}

bool check_msg(const std::string& msg) {
    switch (msg[0]) {
        case '1':
            if (msg.ends_with(".cpp")) {
                return true;
            }
            break;
        case '2':
            if (msg.ends_with("tex")) {
                return true;
            }
            break;
        case '3':
            if (atoi(msg.substr(2).c_str()) > 0) {
                return true;
            }
            break;
        default:
            break;
    }
    std::cout << "Invalid command, try again." << std::endl;
    return false;
}
