#pragma once


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include "errno.h"

#include "../utils/logger/include/logger.hpp"
#include "../utils/tcp/client/include/tcp_client.hpp"



class Client {
public:
    Client();
    void send_msg(const std::string& msg);
    ~Client();
private:
    int client_socket;
    sockaddr_in serverAddress{};
    std::unique_ptr<Logger> logger;
};