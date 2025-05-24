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
#include <utility>

#include "logger.hpp"


struct tcp_traffic_pkg {
    size_t sz;
    char msg[4096];
};

class TcpServer {
public:
    TcpServer();
    void recv_msg(int s);
    int accept_msg();
    int work();
    void determine_msg(const char* msg, int s);
    void disconnect_client();
    // send_task(...) -> отправка серверу таски
    ~TcpServer();
private:
    struct client_info{
        std::thread thread;
        bool is_working;
    };

    mutable std::mutex mutex;
    std::thread disconnect_thread;
    std::map<int, client_info> data;
    int server_socket;
    sockaddr_in serverAddress{};
    std::unique_ptr<Logger> logger;
    // тут sh_mem & semaphore for subprocess

};


