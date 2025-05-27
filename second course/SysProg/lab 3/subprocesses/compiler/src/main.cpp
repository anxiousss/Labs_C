#include "compiler.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <queue_name>" << std::endl;
            return 1;
        }

        std::string queue_name = argv[1];
        CompilerSubprocess compiler(queue_name);
        compiler.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Compiler error: " << e.what() << std::endl;
        return 1;
    }
}