#pragma once
#include "exceptions.hpp"

class TCPClientException : public ExceptionBase {
public:
    explicit TCPClientException(std::string message, log_lvl level = log_lvl::ERROR);
};

class TCPClientConnectionError : public TCPClientException {
public:
    explicit TCPClientConnectionError(const std::string& host, uint16_t port);
};

class TCPClientSendError : public TCPClientException {
public:
    explicit TCPClientSendError();
};