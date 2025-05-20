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

#include "../utils/logger/include/logger.hpp"
#include "../utils/tcp/client/include/tcp_client.hpp"
#include "../utils/tcp/server/include/tcp_server.hpp"
#include "/mnt/c/CLionProjects/sys_prog/lab2/task2/ts_queue.hpp"

class Server {
public:
        Server();
        int recv_msg();
        ~Server();
private:
        //TsQueue<Task> tsQueue{10};
        int server_socket;
        sockaddr_in serverAddress{};
        std::unique_ptr<Logger> logger;
        // тут sh_mem & semaphore for subprocess

};


