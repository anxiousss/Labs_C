#include "tcp_server.hpp"
#include "logger.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        auto logger = LoggerBuilder("ServerMain").set_console().build();
        TCPServer server(8080);

        logger->LogInfo("Starting server on port 8080...");
        server.start();

        std::cout << "Server is running. Press Ctrl+C to stop." << std::endl;

        std::cin.get();

        logger->LogInfo("Stopping server...");
        server.stop();

        return 0;
    } catch (const ExceptionBase& e) {
        e.log(logger);
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
}