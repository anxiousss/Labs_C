
#pragma once


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <thread>
#include <map>

#include "logger.hpp"

class TcpServer {
public:
    TcpServer();
    int recv_msg(int s);
    int accept_msg() {
        int client_socket = accept(server_socket, nullptr, nullptr);
        return client_socket;
    }
    ~TcpServer();
private:
    // std::map<int, std::thread> data;
    int server_socket;
    sockaddr_in serverAddress{};
    std::unique_ptr<Logger> logger;
    // тут sh_mem & semaphore for subprocess

};


