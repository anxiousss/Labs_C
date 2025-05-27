#include "tcp_client.hpp"

TCPClient::TCPClient(const std::string& host, uint16_t port)
        : host_(host), port_(port), socket_fd_(-1),
          logger_(LoggerBuilder("TCPClient").set_console().build()) {
    socket_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ == -1) {
        throw TCPClientConnectionError(host_, port_);
    }

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);

    if (inet_pton(AF_INET, host_.c_str(), &server_addr_.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }
}

TCPClient::~TCPClient() {
    if (socket_fd_ != -1) {
        ::close(socket_fd_);
    }
}

void TCPClient::connect() {
    if (::connect(socket_fd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) == -1) {
        throw TCPClientConnectionError(host_, port_);
    }
    logger_->LogInfo("Connected to server at " + host_ + ":" + std::to_string(port_));
}

void TCPClient::disconnect() {
    if (socket_fd_ != -1) {
        ::close(socket_fd_);
        socket_fd_ = -1;
        logger_->LogInfo("Disconnected from server");
    }
}

void TCPClient::send(const std::string& message) {
    if (socket_fd_ == -1) {
        throw TCPClientSendError();
    }

    try {
        send_size(message.size());

        ssize_t bytes_sent = ::send(socket_fd_, message.data(), message.size(), 0);
        if (bytes_sent == -1) {
            throw TCPClientSendError();
        }
        logger_->LogDebug("Sent " + std::to_string(bytes_sent) + " bytes to server");
    } catch (const std::exception& e) {
        logger_->LogError("Send error: " + std::string(e.what()));
        disconnect();
        throw;
    }
}

std::string TCPClient::receive() {
    if (socket_fd_ == -1) {
        throw TCPClientConnectionError(host_, port_);
    }

    try {
        uint32_t size = receive_size();

        std::vector<char> buffer(size);
        ssize_t bytes_received = ::recv(socket_fd_, buffer.data(), size, 0);

        if (bytes_received == -1) {
            throw std::runtime_error("Receive failed");
        }

        if (bytes_received == 0) {
            logger_->LogInfo("Connection closed by server");
            disconnect();
            return "";
        }

        logger_->LogDebug("Received " + std::to_string(bytes_received) + " bytes from server");
        return std::string(buffer.data(), bytes_received);
    } catch (const std::exception& e) {
        logger_->LogError("Receive error: " + std::string(e.what()));
        disconnect();
        throw;
    }
}

void TCPClient::send_size(uint32_t size) {
    uint32_t net_size = htonl(size);
    ssize_t bytes_sent = ::send(socket_fd_, &net_size, sizeof(net_size), 0);

    if (bytes_sent == -1) {
        throw std::runtime_error("Size send failed");
    }
}

uint32_t TCPClient::receive_size() {
    uint32_t net_size;
    ssize_t bytes_received = ::recv(socket_fd_, &net_size, sizeof(net_size), 0);

    if (bytes_received == -1) {
        throw std::runtime_error("Size receive failed");
    }

    if (bytes_received == 0) {
        logger_->LogInfo("Connection closed by server during size receive");
        disconnect();
        return 0;
    }

    return ntohl(net_size);
}