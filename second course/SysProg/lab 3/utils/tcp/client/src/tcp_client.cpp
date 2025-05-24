

#include "tcp_client.hpp"

TcpClient::TcpClient() {
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

void TcpClient::send_msg(const std::string& msg) {
    const char* str_it = msg.c_str();
    tcp_traffic_pkg tcpTrafficPkg{};

    for (size_t i = 0; i < msg.size(); ++i) {
        tcpTrafficPkg.msg[i] = str_it[i];
    }
    tcpTrafficPkg.sz = msg.size();
    ssize_t err = send(client_socket, &tcpTrafficPkg, sizeof(tcp_traffic_pkg), 0);
    if (err == -1) {
        logger->LogError("send msg error\n");
        throw std::domain_error("send msg error\n");
    }
}

TcpClient::~TcpClient() {
    int err = close(client_socket);
    if (err == -1) {
        logger->LogError("close socket error\n");
    }
}
