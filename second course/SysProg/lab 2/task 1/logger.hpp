#pragma once

#include <string>
#include <mutex>
#include <set>
#include <memory>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class log_lvl {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
public:
    virtual ~Logger() = default;
    virtual void write(const std::string& msg, log_lvl lvl) = 0;
    virtual void close() = 0;

    static bool registerLogger(const std::string& name);
    static void unregisterLogger(const std::string& name);

protected:
    static std::string getCurrentTime();
    std::mutex mut;
    inline static std::mutex time_mutex;
    inline static std::mutex registration_mutex;
    inline static std::set<std::string> existed_loggers;
    inline static std::tm tm_buf;
};

class FileLogger : public Logger {
public:
    FileLogger(std::string  name, log_lvl level, const std::string& file_path);
    void write(const std::string& msg, log_lvl lvl) override;
    void close() override;

private:
    std::string name;
    log_lvl allowed_level;
    std::string file_path;
    std::ofstream file_stream;

    static const char* levelToString(log_lvl lvl);
};

class ConsoleLogger : public Logger {
public:
    ConsoleLogger(std::string name, log_lvl level);
    void write(const std::string& msg, log_lvl lvl) override;
    void close() override;

private:
    std::string name;
    log_lvl allowed_level;
    std::ostream& output;

    static const char* levelToString(log_lvl lvl);
};

class LoggerBuilder {
public:
    explicit LoggerBuilder(std::string name);
    LoggerBuilder& set_level(log_lvl level);
    LoggerBuilder& set_file(const std::string& file_path);
    LoggerBuilder& set_console();
    std::unique_ptr<Logger> build();

private:
    std::string name;
    log_lvl level = log_lvl::INFO;
    std::string file_path;
    bool use_console = false;
};