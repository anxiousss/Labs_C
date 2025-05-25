#pragma once

#include <stdexcept>
#include <string>

class SocketException : public std::runtime_error {
public:
    enum class OperationType {
        SocketCreate,
        Bind,
        Listen,
        Connect,
        Accept,
        Send,
        Recv,
        Close,
        Unknown
    };

    SocketException(OperationType op, int error_code, const std::string& context = "");

    OperationType get_operation_type() const noexcept { return operation; }
    int get_error_code() const noexcept { return error_code; }
    const std::string& get_context() const noexcept { return context_msg; }

private:
    OperationType operation;
    int error_code;
    std::string context_msg;

    static std::string create_message(OperationType op, int err, const std::string& ctx);
};
