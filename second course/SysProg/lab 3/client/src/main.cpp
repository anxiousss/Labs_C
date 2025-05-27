#include "tcp_client.hpp"
#include "tcp_client_exceptions.hpp"
#include "logger.hpp"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>

void print_menu() {
    std::cout << "\n=== Client Menu ===\n";
    std::cout << "1. Compile file\n";
    std::cout << "2. Play Sticks Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice (1-3): ";
}

std::string read_file_content(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handle_compile_mode(TCPClient& client, std::unique_ptr<Logger>& logger) {
    std::string file_path;
    std::cout << "Enter file path to compile: ";
    std::getline(std::cin, file_path);

    try {
        if (!std::filesystem::exists(file_path)) {
            logger->LogWarning("File does not exist: " + file_path);
            std::cout << "Error: File does not exist\n";
            return;
        }

        std::string content = read_file_content(file_path);

        std::string command = "compile " + file_path + " " + content;

        client.send(command);

        std::string response = client.receive();
        logger->LogInfo("Compilation result: " + response);
        std::cout << "Server response: " << response << "\n";
    } catch (const std::exception& e) {
        logger->LogError("Compilation error: " + std::string(e.what()));
        std::cout << "Error: " << e.what() << "\n";
    }
}

void handle_game_mode(TCPClient& client, std::unique_ptr<Logger>& logger) {
    std::cout << "\nStarting Sticks Game!\n";
    std::cout << "Rules: You and the server take turns taking 1-3 sticks.\n";
    std::cout << "The player who takes the last stick wins.\n\n";


    client.send("game start");
    std::string response = client.receive();
    std::cout << "Server: " << response << "\n";

    while (true) {
        std::cout << "Enter number of sticks to take (1-3, or 0 to quit): ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "0") {
            client.send("game quit");
            std::cout << "Game ended by user\n";
            break;
        }

        try {
            int sticks = std::stoi(input);
            if (sticks < 1 || sticks > 3) {
                std::cout << "Please enter a number between 1 and 3\n";
                continue;
            }

            client.send("game take " + input);
            response = client.receive();

            std::cout << "Server: " << response << "\n";


            if (response.find("win") != std::string::npos ||
                response.find("lose") != std::string::npos) {
                break;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Please enter a valid number\n";
        } catch (const std::exception& e) {
            logger->LogError("Game error: " + std::string(e.what()));
            std::cout << "Game error: " + std::string(e.what()) << "\n";
            break;
        }
    }
}

int main() {
    try {
        auto logger = LoggerBuilder("ClientMain").set_console().build();
        TCPClient client("127.0.0.1", 8080);

        logger->LogInfo("Connecting to server...");
        client.connect();

        int choice;
        do {
            print_menu();
            std::string input;
            std::getline(std::cin, input);

            try {
                choice = std::stoi(input);

                switch (choice) {
                    case 1:
                        handle_compile_mode(client, logger);
                        break;
                    case 2:
                        handle_game_mode(client, logger);
                        break;
                    case 3:
                        logger->LogInfo("User selected exit");
                        client.send("exit");
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            } catch (const std::invalid_argument&) {
                std::cout << "Please enter a valid number\n";
                choice = 0;
            } catch (const TCPClientException& e) {
                e.log(logger    );
                std::cout << "Connection error: " << e.what() << "\n";
                choice = 3;
            } catch (const std::exception& e) {
                logger->LogError("Error: " + std::string(e.what()));
                std::cout << "Error: " << e.what() << "\n";
                choice = 3;
            }
        } while (choice != 3);

        client.disconnect();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        return 1;
    }
}