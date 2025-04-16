#include "logger.hpp"


int main() {
    try {
        LoggerBuilder builder("myapp.log");

        Logger log = builder
                .set_level(log_lvl::DEBUG)
                .add_file_handler("logs/myapp.log")
                .add_console_handler()
                .build();


        log.write("=== Session started ===", log_lvl::INFO);
        log.write("Initializing components...", log_lvl::DEBUG);
        log.write("Cache size: 256 MB", log_lvl::INFO);
        log.write("Low memory warning", log_lvl::WARNING);
        log.write("Failed to open device", log_lvl::ERROR);
        log.write("Disk corruption detected!", log_lvl::CRITICAL);
        log.write("=== Session ended ===", log_lvl::INFO);

        log.close();
    }
    catch (const std::exception& e) {
        std::cerr << "\033[31mFatal error: " << e.what() << "\033[0m\n";
        return 1;
    }
    return 0;
}