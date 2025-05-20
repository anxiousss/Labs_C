#include "client.hpp"

Client::Client() {
    LoggerBuilder file_builder("client.log");
    logger = file_builder.set_level(log_lvl::DEBUG).set_file("../logs/client.log").build();
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int err = connect(client_socket, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if (err == -1) {
        perror("ROFLS");
        logger->LogError("connection error\n");
        throw std::domain_error("connection error\n");
    }
}

void Client::send_msg(const std::string& msg) {
    //tcp_traffic_pkg tcpTrafficPkg{ msg.size(), msg.c_str()};
    ssize_t err = send(client_socket, msg.c_str(), msg.size(), 0);
    if (err == -1) {
        logger->LogError("send msg error\n");
        throw std::domain_error("send msg error\n");
    }
}

Client::~Client() {
    int err = close(client_socket);
    if (err == -1) {
        logger->LogError("close socket error\n");
    }
}
