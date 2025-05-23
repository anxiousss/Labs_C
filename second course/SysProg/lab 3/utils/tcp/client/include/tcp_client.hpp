#pragma once


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include "errno.h"

#include "logger.hpp"
#include "tcp_client.hpp"



class TcpClient {
public:
    TcpClient();
    void send_msg(const std::string& msg);
    ~TcpClient();
private:
    int client_socket;
    sockaddr_in serverAddress{};
    std::unique_ptr<Logger> logger;
};