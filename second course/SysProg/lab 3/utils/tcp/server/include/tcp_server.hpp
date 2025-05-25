#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <memory>
#include <filesystem>

#include "logger.hpp"

#include "../../../../ipc/shared_memory/include/shared_memory.hpp"
#include "../../../../ipc/semaphore/include/semaphore.hpp"
#include "../../../../ipc/message_queue/include/message_queue.hpp"

struct tcp_traffic_pkg {
    uint32_t sz;
    char msg[4096 - 4];
};

struct CompileTask {
    int client_socket;
    char file_path[256];
    enum class Type { CPP, TEX } type;
};

class TcpServer {
public:
    TcpServer();
    void start();
    void stop();

private:
    struct ClientInfo {
        std::thread thread;
        std::atomic<bool> is_connected;
        std::string current_file;
        std::vector<char> file_data;

        ClientInfo(std::thread&& t, bool connected, std::string file, std::vector<char> data)
                : thread(std::move(t)),
                  is_connected(connected),
                  current_file(std::move(file)),
                  file_data(std::move(data)) {}

        ClientInfo(const ClientInfo&) = delete;
        ClientInfo& operator=(const ClientInfo&) = delete;
    };

    void handle_client(int client_socket);
    void receive_file(int client_socket);
    void process_compile_task(int client_socket, const std::string& file_path, CompileTask::Type type);
    void process_game_move(int client_socket, int sticks);
    void cleanup_resources();

    std::mutex clients_mutex;
    std::map<int, ClientInfo> clients;
    std::atomic<bool> running;

    int server_fd;
    sockaddr_in address;
    std::unique_ptr<Logger> logger;

    std::unique_ptr<SharedMemory> compile_shm;
    std::unique_ptr<Semaphore> compile_sem;
    std::mutex game_queues_mutex;
    std::map<int, std::unique_ptr<MessageQueue>> game_queues;
};
