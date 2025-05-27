#pragma once
#include <string>
#include <memory>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <system_error>
#include <logger.hpp>
#include "tcp_client_exceptions.hpp"

class TCPClient {
public:
    explicit TCPClient(const std::string& host, uint16_t port);
    ~TCPClient();

    void connect();
    void send(const std::string& message);
    std::string receive();
    void disconnect();

private:
    std::string host_;
    uint16_t port_;
    int socket_fd_;
    sockaddr_in server_addr_;
    std::unique_ptr<Logger> logger_;

    void send_size(uint32_t size);
    uint32_t receive_size();
};