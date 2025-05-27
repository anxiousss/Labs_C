#include "tcp_server_exception.hpp"

TCPServerException::TCPServerException(std::string message, log_lvl level)
        : ExceptionBase(std::move(message), level, "TCPServer") {}

TCPSocketCreationError::TCPSocketCreationError()
        : TCPServerException("Failed to create socket", log_lvl::CRITICAL) {}

TCPServerStartError::TCPServerStartError(uint16_t port)
        : TCPServerException("Failed to start server on port " + std::to_string(port), log_lvl::CRITICAL) {}

TCPBindError::TCPBindError(uint16_t port)
        : TCPServerException("Failed to bind socket to port " + std::to_string(port), log_lvl::CRITICAL) {}

TCPListenError::TCPListenError(uint16_t port)
        : TCPServerException("Failed to listen on port " + std::to_string(port), log_lvl::CRITICAL) {}

TCPAcceptError::TCPAcceptError()
        : TCPServerException("Failed to accept connection", log_lvl::ERROR) {}
