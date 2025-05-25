#pragma once


#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "logger.hpp"


struct tcp_traffic_pkg {
    uint32_t sz;
    char msg[4096 - 4];
};

class TcpClient {
public:
    TcpClient();
    void send_msg(const std::string& msg);
    void send_file(const std::string& path);
    std::string receive_response();
    ~TcpClient();
private:
    int client_socket;
    sockaddr_in serverAddress{};
    std::unique_ptr<Logger> logger;
};