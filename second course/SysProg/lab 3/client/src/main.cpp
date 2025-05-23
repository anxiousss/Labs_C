#include "client.hpp"
#include "tcp_client.hpp"

int main() {
    TcpClient tcp_client;
    std::string msg;
    interface();
    while (true) {
        std::cin >> msg;
        if (msg == "exit") {
            break;
        }
        //if (!check_msg(msg)) continue;
        tcp_client.send_msg(msg);
        interface();
    }
    return 0;
}