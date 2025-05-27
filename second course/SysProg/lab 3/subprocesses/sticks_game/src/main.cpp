#include "game_worker.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <key>" << std::endl;
            return 1;
        }

        key_t key = std::stol(argv[1]);
        GameSubprocess game(key);
        game.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Game error: " + std::string(e.what()) << std::endl;
        return 1;
    }
}