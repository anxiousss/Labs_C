#include "tcp_client.hpp"
#include "client.hpp"
#include <iostream>
#include <string>

int main() {
    try {
        TcpClient client;
        interface();

        while(true) {
            std::string msg;
            std::getline(std::cin, msg);

            if(msg == "exit") {
                client.send_msg(msg);
                break;
            }

            if(!check_msg(msg)) {
                std::cout << "Invalid command format!" << std::endl;
                interface();
                continue;
            }

            if(msg[0] == '1' || msg[0] == '2') {
                client.send_msg(msg);
                client.send_file(msg.substr(2));
            } else {
                client.send_msg(msg);
            }

            auto response = client.receive_response();
            std::cout << "Server response: " << response << std::endl;
        }

    } catch(const std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}