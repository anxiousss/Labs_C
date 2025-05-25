#include "tcp_client.hpp"
#include "exceptions.hpp"
#include <fstream>
#include <filesystem>
#include <system_error>
#include <cstring>

namespace fs = std::filesystem;

TcpClient::TcpClient() {
    LoggerBuilder builder("client");
    logger = builder.set_level(log_lvl::DEBUG)
            .set_file("../logs/client.log")
            .build();

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        logger->LogError("Socket creation failed");
        throw SocketException(SocketException::OperationType::SocketCreate, errno);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        logger->LogError("Invalid address");
        throw SocketException(SocketException::OperationType::Connect, errno);
    }

    if (connect(client_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        logger->LogError("Connection failed");
        throw SocketException(SocketException::OperationType::Connect, errno);
    }

    logger->LogInfo("Connected to server");
}

void TcpClient::send_msg(const std::string& msg) {
    tcp_traffic_pkg pkg{};
    pkg.sz = msg.size();
    std::memcpy(pkg.msg, msg.data(), msg.size());

    ssize_t bytes_sent = send(client_socket, &pkg, sizeof(pkg), 0);
    if (bytes_sent < 0) {
        logger->LogError("Message send failed");
        throw SocketException(SocketException::OperationType::Send, errno);
    }
    logger->LogDebug("Sent message: " + msg);
}

void TcpClient::send_file(const std::string& file_path) {
    try {
        fs::path path(file_path);
        if (!fs::exists(path)) {
            logger->LogError("File not found: " + file_path);
            throw std::runtime_error("File not found");
        }

        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        size_t file_size = file.tellg();
        file.seekg(0);

        tcp_traffic_pkg pkg{};
        while (!file.eof()) {
            file.read(pkg.msg, sizeof(pkg.msg));
            pkg.sz = file.gcount();

            if (send(client_socket, &pkg, sizeof(pkg), 0) < 0) {
                logger->LogError("File send failed");
                throw SocketException(SocketException::OperationType::Send, errno);
            }
        }
        logger->LogInfo("File sent: " + file_path + " (" + std::to_string(file_size) + " bytes)");
    } catch (const fs::filesystem_error& e) {
        logger->LogError("Filesystem error: " + std::string(e.what()));
        throw;
    }
}

std::string TcpClient::receive_response() {
    tcp_traffic_pkg pkg{};
    ssize_t bytes_received = recv(client_socket, &pkg, sizeof(pkg), 0);

    if (bytes_received < 0) {
        logger->LogError("Receive failed");
        throw SocketException(SocketException::OperationType::Recv, errno);
    }
    if (bytes_received == 0) {
        logger->LogWarning("Connection closed by server");
        return "[CONNECTION CLOSED]";
    }

    std::string response(pkg.msg, pkg.sz);
    logger->LogDebug("Received response: " + response);
    return response;
}

TcpClient::~TcpClient() {
    if (client_socket != -1) {
        try {
            send_msg("exit");
            shutdown(client_socket, SHUT_RDWR);
            close(client_socket);
        } catch (...) {
            logger->LogError("Error during client shutdown");
        }
    }
    logger->LogInfo("Client shutdown complete");
}