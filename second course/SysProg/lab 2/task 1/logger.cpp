#include "logger.hpp"
#include <filesystem>
#include <utility>

bool Logger::registerLogger(const std::string& name) {
    std::lock_guard<std::mutex> lock(registration_mutex);
    return existed_loggers.insert(name).second;
}

void Logger::unregisterLogger(const std::string& name) {
    std::lock_guard<std::mutex> lock(registration_mutex);
    existed_loggers.erase(name);
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;

    {
        std::lock_guard<std::mutex> lock(time_mutex);
        tm_buf = *std::localtime(&t);
    }

    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S")
       << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

const char* Logger::level_info(log_lvl lvl) {
    switch(lvl) {
        case log_lvl::DEBUG: return "DEBUG";
        case log_lvl::INFO: return "INFO";
        case log_lvl::WARNING: return "WARNING";
        case log_lvl::ERROR: return "ERROR";
        case log_lvl::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

FileLogger::FileLogger(std::string  name_, log_lvl level_, std::string  file_path_)
        : Logger(level_), name(std::move(name_)), file_path(std::move(file_path_)) {
    if (!registerLogger(name)) {
        throw std::logic_error("Logger '" + name + "' already exists");
    }

    namespace fs = std::filesystem;
    fs::path path_obj(file_path);
    if (auto parent_path = path_obj.parent_path(); !parent_path.empty()) {
        fs::create_directories(parent_path);
    }

    file_stream.open(file_path, std::ios::app);
    if (!file_stream.is_open()) {
        unregisterLogger(name);
        throw std::runtime_error("Failed to open log file: " + file_path);
    }
}

void FileLogger::write(const std::string& msg, log_lvl lvl) {
    if (lvl < allowed_level) return;

    std::lock_guard<std::mutex> lock(mut);
    if (!file_stream.is_open()) {
        throw std::runtime_error("Log file '" + file_path + "' is closed");
    }

    file_stream << "[" << getCurrentTime() << "] "
                << "[" << level_info(lvl) << "] "
                << msg << std::endl;
}

void FileLogger::close() {
    std::lock_guard<std::mutex> lock(mut);
    if (file_stream.is_open()) {
        file_stream.close();
    }
    unregisterLogger(name);
}

ConsoleLogger::ConsoleLogger(std::string  name_, log_lvl level_)
        : Logger(level_), name(std::move(name_)) {
    if (!registerLogger(name)) {
        throw std::logic_error("Logger '" + name + "' already exists");
    }
}

void ConsoleLogger::write(const std::string& msg, log_lvl lvl) {
    if (lvl < allowed_level) return;

    std::lock_guard<std::mutex> lock(mut);
    output << "[" << getCurrentTime() << "] "
           << "[" << level_info(lvl) << "] "
           << msg << std::endl;
}

void ConsoleLogger::close() {
    std::lock_guard<std::mutex> lock(mut);
    output.flush();
    unregisterLogger(name);
}

LoggerBuilder::LoggerBuilder(std::string name_) : name(std::move(name_)) {}

LoggerBuilder& LoggerBuilder::set_level(log_lvl level_) {
    this->level = level_;
    return *this;
}

LoggerBuilder& LoggerBuilder::set_file(const std::string& file_path_) {
    this->file_path = file_path_;
    return *this;
}

LoggerBuilder& LoggerBuilder::set_console() {
    use_console = true;
    return *this;
}

std::unique_ptr<Logger> LoggerBuilder::build() {
    if (!file_path.empty() && use_console) {
        throw std::logic_error("Cannot set both file and console handlers");
    }

    if (!file_path.empty()) {
        return std::make_unique<FileLogger>(name, level, file_path);
    } else if (use_console) {
        return std::make_unique<ConsoleLogger>(name, level);
    } else {
        throw std::logic_error("No handlers specified");
    }
}