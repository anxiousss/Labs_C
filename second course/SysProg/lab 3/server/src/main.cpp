#include "server.hpp"

int main() {

    Server server;
    while (true) {
        server.recv_msg();
    }

    return 0;
}