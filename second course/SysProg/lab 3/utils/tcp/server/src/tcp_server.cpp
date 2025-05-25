#include "tcp_server.hpp"
#include "exceptions.hpp"
#include <fstream>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

TcpServer::TcpServer() : running(false) {
    LoggerBuilder builder("server");
    logger = builder.set_level(log_lvl::DEBUG)
            .set_file("../logs/server.log")
            .build();

    try {
        compile_shm = std::make_unique<SharedMemory>("/tmp", 'C', sizeof(CompileTask));
        compile_sem = std::make_unique<Semaphore>("/tmp", 'S', 0);
    } catch (const std::exception& e) {
        logger->LogCritic("IPC initialization failed: " + std::string(e.what()));
        throw;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        logger->LogError("Socket creation failed");
        throw SocketException(SocketException::OperationType::SocketCreate, errno);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        logger->LogError("Socket configuration failed");
        throw SocketException(SocketException::OperationType::Bind, errno);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        logger->LogError("Socket bind failed");
        throw SocketException(SocketException::OperationType::Bind, errno);
    }

    if (listen(server_fd, 10) < 0) {
        logger->LogError("Listen failed");
        throw SocketException(SocketException::OperationType::Listen, errno);
    }

    running = true;
    logger->LogInfo("Server started on port 8080");
}

void TcpServer::start() {
    while (running) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            if (running) logger->LogError("Accept failed");
            continue;
        }

        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.try_emplace(
                client_socket,
                std::thread(&TcpServer::handle_client, this, client_socket),
                true,
                "",
                std::vector<char>{}
        );
    }
}

void TcpServer::handle_client(int client_socket) {
    try {
        while (true) {
            tcp_traffic_pkg pkg{};
            int bytes_received = recv(client_socket, &pkg, sizeof(pkg), 0);

            if (bytes_received <= 0) {
                if (bytes_received == 0) logger->LogInfo("Client disconnected");
                else logger->LogError("Receive error");
                break;
            }

            std::string message(pkg.msg, pkg.sz);
            logger->LogDebug("Received: " + message);

            if (message.substr(0, 2) == "1 " || message.substr(0, 2) == "2 ") {
                receive_file(client_socket);
            }
            else if (message.substr(0, 2) == "3 ") {
                int sticks = std::stoi(message.substr(2));
                process_game_move(client_socket, sticks);
            }
            else if (message == "exit") {
                break;
            }
        }
    } catch (const std::exception& e) {
        logger->LogError("Client handler error: " + std::string(e.what()));
    }

    std::lock_guard<std::mutex> lock(clients_mutex);
    if (auto it = clients.find(client_socket); it != clients.end()) {
        close(client_socket);
        clients.erase(it);
    }
}

void TcpServer::receive_file(int client_socket) {
    try {
        std::unique_lock<std::mutex> lock(clients_mutex);
        auto it = clients.find(client_socket);
        if (it == clients.end()) return;

        auto& client = it->second;
        lock.unlock();

        fs::path temp_dir = fs::temp_directory_path() / "server_files";
        fs::create_directories(temp_dir);
        fs::path file_path = temp_dir / client.current_file;

        std::ofstream file(file_path, std::ios::binary);
        if (!file) throw std::runtime_error("Failed to create file");

        tcp_traffic_pkg pkg{};
        do {
            int bytes = recv(client_socket, &pkg, sizeof(pkg), 0);
            if (bytes <= 0) break;
            file.write(pkg.msg, pkg.sz);
        } while (pkg.sz == sizeof(pkg.msg));

        file.close();
        logger->LogInfo("File received: " + file_path.string());

        CompileTask::Type type = file_path.extension() == ".cpp" ?
                                 CompileTask::Type::CPP : CompileTask::Type::TEX;

        process_compile_task(client_socket, file_path.string(), type);
    } catch (const std::exception& e) {
        logger->LogError("File receive error: " + std::string(e.what()));
    }
}

void TcpServer::process_compile_task(int client_socket, const std::string& file_path, CompileTask::Type type) {
    CompileTask task{};
    task.client_socket = client_socket;
    strncpy(task.file_path, file_path.c_str(), sizeof(task.file_path));
    task.type = type;

    memcpy(compile_shm->getData(), &task, sizeof(task));
    compile_sem->post();
    logger->LogInfo("Compile task queued: " + file_path);
}
void TcpServer::process_game_move(int client_socket, int sticks) {
    MessageQueue* mq_ptr = nullptr;
    {
        std::lock_guard<std::mutex> lock(game_queues_mutex);

        if (!game_queues.contains(client_socket)) {
            game_queues[client_socket] = std::make_unique<MessageQueue>("/tmp", client_socket);
        }

        mq_ptr = game_queues[client_socket].get();
    }

    mq_ptr->send(&sticks, sizeof(sticks));

    int response;
    mq_ptr->receive(&response, sizeof(response));

    std::string msg = "Sticks left: " + std::to_string(response);
    tcp_traffic_pkg pkg{};
    pkg.sz = msg.size();
    memcpy(pkg.msg, msg.c_str(), msg.size());
    send(client_socket, &pkg, sizeof(pkg), 0);
}

void TcpServer::stop() {
    running = false;
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
    cleanup_resources();
    logger->LogInfo("Server stopped");
}

void TcpServer::cleanup_resources() {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& [socket, client] : clients) {
        if (client.thread.joinable()) {
            client.thread.join();
        }
        close(socket);
    }
    clients.clear();

    game_queues.clear();
    compile_shm.reset();
    compile_sem.reset();
}

