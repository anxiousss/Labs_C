#include "tcp_server.hpp"

TcpServer::TcpServer() {
    LoggerBuilder file_builder("server.log");
    logger = file_builder.set_level(log_lvl::DEBUG).set_file("../logs/server.log").build();
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int err = bind(server_socket, (struct sockaddr*)& serverAddress, sizeof(serverAddress));
    if (err == -1) {
        perror("ROFLS");
        logger->LogError("Bind socket error\n");
        throw std::domain_error("Bind socket error\n");
    }
    listen(server_socket, 5);

    //std::lock_guard<std::mutex> lock(mutex);
    disconnect_thread = std::thread([this]() { disconnect_client();});
}

void TcpServer::recv_msg(int s) {
    while (true) {
        std::cout << "TEST1\n";
        tcp_traffic_pkg tcpTrafficPkg{};
        int err = recv(s, &tcpTrafficPkg, sizeof(tcp_traffic_pkg), 0);
        if (err == -1) {
            logger->LogError("Receive msg error\n");
            throw std::domain_error("Receive msg error\n");
        } else if (err == 0) {
            data[s].is_working = false;
            return;
        }
        std::cout << tcpTrafficPkg.msg << std::endl;
        determine_msg(tcpTrafficPkg.msg, s);
    }
}

TcpServer::~TcpServer() {
    int err = close(server_socket);
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& it: data) {
        if (it.second.thread.joinable()) {
            it.second.thread.join();
        }
    }
    if (err == -1) {
        logger->LogError("Close socket error\n");
    }
}

int TcpServer::accept_msg() {
    std::cout << "TEST2\n";
    int client_socket = accept(server_socket, nullptr, nullptr);
    return client_socket;
}

int TcpServer::work() {
    while (true) {
        int s = accept_msg();
        std::cout << s << std::endl;
        if (s < 0) {
            // log error;
        }
        std::lock_guard<std::mutex> lock(mutex);
        data[s] = {std::thread(&TcpServer::recv_msg, this, s), true};
    }
}

void TcpServer::determine_msg(const char* msg, int s) {
    std::string str(msg);
    if (str.empty()) {
        return;
    }
    if (str == "exit") {
        std::lock_guard<std::mutex> lock(mutex);
        data[s].is_working = false;
        return;
    }
    str = str.substr(2);
    if (str.ends_with(".cpp") || str.ends_with(".tex")) {
        // на подсервер
    } else if (atoi(str.c_str()) > 0) {
        // на подсервер игры
    }
}

void TcpServer::disconnect_client() {
    while (true) {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto& it: data) {
            if (!it.second.is_working && it.second.thread.joinable()) {
                it.second.thread.join();
                data.erase(it.first);
            }
        }
    }
}


