#include "logger.hpp"

Logger::Logger(std::string path_, log_lvl level,
               std::ostream* handler_,
               std::unique_ptr<std::ofstream> file_stream_)
        : path(std::move(path_)),
          allowed_lvl(level),
          handler(handler_),
          file_stream(std::move(file_stream_)) {}

void Logger::write(const std::string& msg, log_lvl lvl) {
    std::lock_guard<std::mutex> lock(mut);

    if (!handler) {
        throw std::runtime_error("Attempt to write to closed logger");
    }

    if (lvl > allowed_lvl) return;

    const char* level_str = "";
    switch(lvl) {
        case log_lvl::DEBUG:    level_str = "DEBUG"; break;
        case log_lvl::INFO:     level_str = "INFO"; break;
        case log_lvl::WARNING:  level_str = "WARNING"; break;
        case log_lvl::ERROR:    level_str = "ERROR"; break;
        case log_lvl::CRITICAL: level_str = "CRITICAL"; break;
    }

    if (handler) {
        *handler << "[" << level_str << "] " << msg << std::endl;
    }
}

void Logger::close(LoggerBuilder& builder) {
    std::lock_guard<std::mutex> lock(mut);
    file_stream.reset();
    handler = nullptr;
    builder.existed_loggers.erase(path);
}

LoggerBuilder::LoggerBuilder(std::string path_) : path(std::move(path_)) {}

LoggerBuilder& LoggerBuilder::set_level(log_lvl level_) {
    this->level = level_;
    return *this;
}

LoggerBuilder& LoggerBuilder::add_file_handler(const std::string& file_path) {
    namespace fs = std::filesystem;

    try {
        fs::path path_obj(file_path);
        if (auto parent_path = path_obj.parent_path(); !parent_path.empty()) {
            fs::create_directories(parent_path);
        }

        current_file_stream = std::make_unique<std::ofstream>(
                file_path, std::ios::app | std::ios::out
        );

        if (!current_file_stream->is_open()) {
            throw std::runtime_error("Cannot open file: " + file_path);
        }

        current_handler = current_file_stream.get();
    }
    catch (const fs::filesystem_error& e) {
        throw std::runtime_error("Filesystem error: " + std::string(e.what()));
    }
    return *this;
}

LoggerBuilder& LoggerBuilder::add_console_handler() {
    current_handler = &std::cout;
    return *this;
}

Logger LoggerBuilder::build() {
    if (existed_loggers.count(path)) {
        throw std::logic_error("Logger already exists");
    }
    if (!current_handler) {
        throw std::logic_error("No handlers added to logger");
    }
    existed_loggers.insert(path);

    return Logger{
            path,
            level,
            current_handler,
            std::move(current_file_stream)
    };
}