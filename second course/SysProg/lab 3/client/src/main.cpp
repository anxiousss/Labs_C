#include "client.hpp"
#include "tcp_client.hpp"

int main() {
    TcpClient tcp_client;
    std::string msg;
    interface();
    while (true) {
        std::cin >> msg;
        //if (!check_msg(msg)) continue;
        tcp_client.send_msg(msg);
        interface();
        if (msg == "exit") {
            break;
        }
    }
    return 0;
}