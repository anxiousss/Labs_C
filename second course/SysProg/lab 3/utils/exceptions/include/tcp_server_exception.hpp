#pragma once
#include "exceptions.hpp"

class TCPServerException : public ExceptionBase {
public:
    explicit TCPServerException(std::string message, log_lvl level = log_lvl::ERROR);
};

class TCPServerStartError : public TCPServerException {
public:
    explicit TCPServerStartError(uint16_t port);
};

class TCPSocketCreationError : public TCPServerException {
public:
    TCPSocketCreationError();
};

class TCPBindError : public TCPServerException {
public:
    explicit TCPBindError(uint16_t port);
};

class TCPListenError : public TCPServerException {
public:
    explicit TCPListenError(uint16_t port);
};

class TCPAcceptError : public TCPServerException {
public:
    TCPAcceptError();
};