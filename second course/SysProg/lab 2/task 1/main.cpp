#include "logger.hpp"

int main() {
    try {
        LoggerBuilder console_builder("console_log");
        auto console_log = console_builder
                .set_level(log_lvl::INFO)
                .set_console()
                .build();

        console_log->LogInfo("Application initialization");
        console_log->LogDebug("This debug message won't appear");
        console_log->LogWarning("SSL certificate expires soon");
        console_log->close();

        LoggerBuilder file_builder("myapp.log");
        auto file_log = file_builder
                .set_level(log_lvl::DEBUG)
                .set_file("logs/myapp.log")
                .build();

        file_log->LogDebug("Starting data processing");
        file_log->LogInfo("Processed 235 records");
        file_log->LogError("DB connection timeout");
        file_log->close();

    }
    catch (const std::exception& e) {
        std::cerr << "Main error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}