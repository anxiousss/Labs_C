#include "logger.hpp"

int main() {
    try {
        LoggerBuilder builder("myapp.log");

        Logger log1 = builder
                .set_level(log_lvl::INFO)
                .add_console_handler()
                .build();

        log1.write("Test message 1", log_lvl::INFO);
        log1.close(builder);

        Logger log2 = builder
                .set_level(log_lvl::ERROR)
                .add_file_handler("logs/myapp.log")
                .build();

        log2.write("Critical error", log_lvl::CRITICAL);
        log2.close(builder);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}