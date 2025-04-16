#include "logger.hpp"
#include <stdexcept>
#include <utility>
#include <filesystem>


Logger::Logger(std::string  path_,
               log_lvl level,
               std::vector<std::ostream*> handlers_,
               std::vector<std::unique_ptr<std::ofstream>> file_streams_)
        : path(std::move(path_)),
          allowed_lvl(level),
          handlers(std::move(handlers_)),
          file_streams(std::move(file_streams_)) {}

void Logger::write(const std::string& msg, log_lvl lvl) {
    std::lock_guard<std::mutex> lock(mut);
    if (lvl < allowed_lvl) return;

    const char* level_str = "";
    switch(lvl) {
        case log_lvl::DEBUG:    level_str = "DEBUG"; break;
        case log_lvl::INFO:     level_str = "INFO"; break;
        case log_lvl::WARNING:  level_str = "WARNING"; break;
        case log_lvl::ERROR:    level_str = "ERROR"; break;
        case log_lvl::CRITICAL: level_str = "CRITICAL"; break;
    }

    for (auto& handler : handlers) {
        if (handler) {
            *handler << "[" << level_str << "] " << msg << std::endl;
        }
    }
}

void Logger::close() {
    std::lock_guard<std::mutex> lock(mut);
    file_streams.clear();
    handlers.clear();
}

LoggerBuilder::LoggerBuilder(std::string  path_) : path(std::move(path_)) {}

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

        auto fs = std::make_unique<std::ofstream>(
                file_path,
                std::ios::app | std::ios::out
        );

        if (!fs->is_open()) {
            throw std::runtime_error(
                    "Cannot open file: " + file_path +
                    "\nPossible reasons:\n"
                    "1. Invalid path format\n"
                    "2. Missing permissions\n"
                    "3. Filesystem is read-only"
            );
        }

        handlers.push_back(fs.get());
        file_streams.push_back(std::move(fs));
    }
    catch (const fs::filesystem_error& e) {
        throw std::runtime_error(
                "Filesystem error: " + std::string(e.what()) +
                "\nPath: " + file_path
        );
    }
    return *this;
}

LoggerBuilder& LoggerBuilder::add_console_handler() {
    handlers.push_back(&std::cout);
    return *this;
}

Logger LoggerBuilder::build() {
    if (!existed_loggers.insert(path).second) {
        throw std::logic_error("Logger already exists");
    }
    return {path, level, handlers, std::move(file_streams)};
}