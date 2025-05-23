#include "server.hpp"

int main() {

    TcpServer tcp_server;
    while (true) {
        int s = tcp_server.accept_msg();
        while(true) {
            tcp_server.recv_msg(s);
        }
    }

    return 0;
}