#include "../include/tcp_client_exceptions.hpp"

TCPClientException::TCPClientException(std::string message, log_lvl level)
        : ExceptionBase(std::move(message), level, "TCPClient") {}

TCPClientConnectionError::TCPClientConnectionError(const std::string& host, uint16_t port)
        : TCPClientException("Failed to connect to server at " + host + ":" + std::to_string(port), log_lvl::ERROR) {}

TCPClientSendError::TCPClientSendError()
        : TCPClientException("Failed to send data to server", log_lvl::ERROR) {}