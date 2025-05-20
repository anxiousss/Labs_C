#include "../include/client.hpp"

int main() {
    Client client;
    std::string msg;
    interface();
    while (true) {
        std::cin >> msg;
        if (msg == "exit") {
            break;
        }
        //if (!check_msg(msg)) continue;
        client.send_msg(msg);
        interface();
    }
    return 0;
}