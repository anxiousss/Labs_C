#pragma once

#include <string>
#include <mutex>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

enum class log_lvl {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
public:
    void write(const std::string& msg, log_lvl lvl);
    void close();

private:
    friend class LoggerBuilder;
    Logger(std::string  path_,
           log_lvl level,
           std::vector<std::ostream*> handlers,
           std::vector<std::unique_ptr<std::ofstream>> file_streams);

    std::mutex mut;
    std::string path;
    log_lvl allowed_lvl;
    std::vector<std::ostream*> handlers;
    std::vector<std::unique_ptr<std::ofstream>> file_streams;
};

class LoggerBuilder {
public:
    explicit LoggerBuilder(std::string path);
    LoggerBuilder& set_level(log_lvl level);
    LoggerBuilder& add_file_handler(const std::string& file_path);
    LoggerBuilder& add_console_handler();
    Logger build();

private:
    std::string path;
    log_lvl level = log_lvl::INFO;
    std::vector<std::ostream*> handlers;
    std::vector<std::unique_ptr<std::ofstream>> file_streams;
    inline static std::set<std::string> existed_loggers;
};