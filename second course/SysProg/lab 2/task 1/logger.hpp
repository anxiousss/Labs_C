#pragma once

#include <string>
#include <mutex>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <filesystem>

enum class log_lvl {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class LoggerBuilder;

class Logger {
public:
    void write(const std::string& msg, log_lvl lvl);
    void close(LoggerBuilder& builder);

private:
    friend class LoggerBuilder;
    Logger(std::string path_, log_lvl level,
           std::ostream* handler,
           std::unique_ptr<std::ofstream> file_stream);

    std::mutex mut;
    std::string path;
    log_lvl allowed_lvl;
    std::ostream* handler;
    std::unique_ptr<std::ofstream> file_stream;
};

class LoggerBuilder {
public:
    explicit LoggerBuilder(std::string path);
    LoggerBuilder& set_level(log_lvl level);
    LoggerBuilder& add_file_handler(const std::string& file_path);
    LoggerBuilder& add_console_handler();
    Logger build();

private:
    friend class Logger;
    std::string path;
    log_lvl level = log_lvl::INFO;
    std::ostream* current_handler = nullptr;
    std::unique_ptr<std::ofstream> current_file_stream;
    inline static std::set<std::string> existed_loggers;
};