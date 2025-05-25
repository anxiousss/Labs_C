#include "tcp_server.hpp"
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main() {
    try {
        TcpServer server;
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        std::thread server_thread([&server](){
            server.start();
        });

        while(running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        server.stop();
        if(server_thread.joinable()) {
            server_thread.join();
        }

    } catch(const std::exception& e) {
        std::cerr << "Server fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}