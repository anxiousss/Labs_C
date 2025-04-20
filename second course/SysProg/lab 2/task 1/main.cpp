#include "logger.hpp"
#include <memory>

int main() {
    try {
        LoggerBuilder builder1("myapp.log");
        auto log1 = builder1.set_level(log_lvl::INFO).set_console().build();
        log1->write("Test message 1", log_lvl::INFO);
        log1->close();

        LoggerBuilder builder2("myapp.log");
        auto log2 = builder2.set_level(log_lvl::ERROR).set_file("logs/myapp.log").build();
        log2->write("Critical error", log_lvl::ERROR);
        log2->close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}