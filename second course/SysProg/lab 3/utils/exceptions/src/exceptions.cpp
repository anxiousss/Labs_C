#include "exceptions.hpp"
#include <sstream>
#include <cstring>

SocketException::SocketException(OperationType op, int error_code, const std::string& context)
        : std::runtime_error(create_message(op, error_code, context)),
          operation(op),
          error_code(error_code),
          context_msg(context) {}

std::string SocketException::create_message(OperationType op, int err, const std::string& ctx) {
    std::ostringstream ss;

    ss << "Socket error during: ";
    switch(op) {
        case OperationType::SocketCreate: ss << "socket()"; break;
        case OperationType::Bind:        ss << "bind()"; break;
        case OperationType::Listen:      ss << "listen()"; break;
        case OperationType::Connect:     ss << "connect()"; break;
        case OperationType::Accept:      ss << "accept()"; break;
        case OperationType::Send:        ss << "send()"; break;
        case OperationType::Recv:        ss << "recv()"; break;
        case OperationType::Close:       ss << "close()"; break;
        default:                         ss << "unknown operation"; break;
    }

    ss << "\nError code: " << err
       << "\nSystem message: " << std::strerror(err);

    if(!ctx.empty()) {
        ss << "\nContext: " << ctx;
    }

    return ss.str();
}