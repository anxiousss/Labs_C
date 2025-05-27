#include "tcp_server.hpp"
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <system_error>

TCPServer::TCPServer(uint16_t port)
        : port_(port), server_fd_(-1), running_(false),
          logger_(LoggerBuilder("TCPServer").set_console().build()) {
    server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1) {
        throw TCPSocketCreationError();
    }

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)) == -1) {
        ::close(server_fd_);
        throw TCPSocketCreationError();
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        ::close(server_fd_);
        throw TCPBindError(port_);
    }
}

TCPServer::~TCPServer() {
    stop();
    if (server_fd_ != -1) {
        ::close(server_fd_);
    }
}

void TCPServer::start() {
    if (running_) {
            throw TCPServerStartError(port_);
    }

    if (listen(server_fd_, SOMAXCONN) == -1) {
        throw TCPListenError(port_);
    }

    running_ = true;
    logger_->LogInfo("Server started on port " + std::to_string(port_));

    std::thread accept_thread([this]() { this->accept_loop(); });
    accept_thread.detach();
}

void TCPServer::stop() {
    if (!running_) {
        return;
    }

    running_ = false;

    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        for (int fd : client_fds_) {
            ::close(fd);
        }
        client_fds_.clear();
    }

    if (server_fd_ != -1) {
        ::shutdown(server_fd_, SHUT_RDWR);
        ::close(server_fd_);
        server_fd_ = -1;
    }

    logger_->LogInfo("Server stopped");
}

void TCPServer::accept_loop() {
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = ::accept(server_fd_, (struct sockaddr*)&client_addr, &addr_len);

        if (client_fd == -1) {
            if (!running_) {
                break;
            }


            if (errno == EINTR && !running_) {
                break;
            }

            logger_->LogError("Accept failed");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            client_fds_.push_back(client_fd);
        }

        logger_->LogInfo("New client connected: " + std::string(inet_ntoa(client_addr.sin_addr)) +
                         ":" + std::to_string(ntohs(client_addr.sin_port)));

        std::thread client_thread([this, client_fd]() { this->handle_client(client_fd); });
        client_thread.detach();
    }
}

void TCPServer::handle_client(int client_fd) {
    try {
        while (true) {
            uint32_t net_size;
            ssize_t bytes_read = ::recv(client_fd, &net_size, sizeof(net_size), 0);

            if (bytes_read == 0) {
                logger_->LogInfo("Client disconnected");
                break;
            }

            if (bytes_read == -1) {
                logger_->LogError("Receive failed");
                break;
            }

            uint32_t size = ntohl(net_size);

            std::vector<char> buffer(size);
            bytes_read = ::recv(client_fd, buffer.data(), size, 0);

            if (bytes_read == 0) {
                logger_->LogInfo("Client disconnected");
                break;
            }

            if (bytes_read == -1) {
                logger_->LogError("Receive failed");
                break;
            }

            std::string message(buffer.data(), bytes_read);
            logger_->LogDebug("Received command: " + message);

            std::string response;

            // TODO: Implement command routing to subprocesses
            if (message.find("compile") == 0) {
                response = "Processing compile command: " + message;
            } else if (message.find("game") == 0) {
                response = "Processing game command: " + message;
            } else {
                response = "Unknown command";
            }

            uint32_t net_response_size = htonl(response.size());
            ::send(client_fd, &net_response_size, sizeof(net_response_size), 0);
            ::send(client_fd, response.data(), response.size(), 0);
        }
    } catch (const ExceptionBase& e) {
        e.log(logger_);
    } catch (const std::exception& e) {
        logger_->LogError("Client handler error: " + std::string(e.what()));
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        auto it = std::find(client_fds_.begin(), client_fds_.end(), client_fd);
        if (it != client_fds_.end()) {
            client_fds_.erase(it);
        }
    }
    ::close(client_fd);
}