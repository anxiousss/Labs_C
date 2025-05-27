#pragma once
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <logger.hpp>
#include <tcp_server_exception.hpp>

class TCPServer {
public:
    explicit TCPServer(uint16_t port);
    ~TCPServer();

    void start();
    void stop();

private:
    void accept_loop();
    void handle_client(int client_fd);

    uint16_t port_;
    int server_fd_;
    std::vector<std::thread> worker_threads_;
    std::mutex clients_mutex_;
    std::vector<int> client_fds_;
    bool running_;
    std::unique_ptr<Logger> logger_;
};