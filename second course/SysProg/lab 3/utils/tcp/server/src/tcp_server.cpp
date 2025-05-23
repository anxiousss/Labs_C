#include "tcp_server.hpp"

TcpServer::TcpServer() {
    LoggerBuilder file_builder("server.log");
    logger = file_builder.set_level(log_lvl::DEBUG).set_file("../logs/server.log").build();
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int err = bind(server_socket, (struct sockaddr*)& serverAddress, sizeof(serverAddress));
    if (err == -1) {
        perror("ROFLS");
        logger->LogError("Bind socket error\n");
        throw std::domain_error("Bind socket error\n");
    }
    listen(server_socket, 5);
}

int TcpServer::recv_msg(int s) {
    char buffer[4096] = {0};
    int err = recv(s, buffer, sizeof(buffer), 0);
    if(err == -1) {
        logger->LogError("Receive msg error\n");
        throw std::domain_error("Receive msg error\n");
    }
    std::cout << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));
    return err;
}

TcpServer::~TcpServer() {
    int err = close(server_socket);
    if (err == -1) {
        logger->LogError("Close socket error\n");
    }
}


