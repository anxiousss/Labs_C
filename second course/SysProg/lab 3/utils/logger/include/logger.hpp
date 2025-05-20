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

    virtual void LogDebug(const std::string& msg) { write(msg, log_lvl::DEBUG); }
        virtual void LogInfo(const std::string& msg)  { write(msg, log_lvl::INFO); }
    virtual void LogWarning(const std::string& msg) { write(msg, log_lvl::WARNING); }
    virtual void LogError(const std::string& msg) { write(msg, log_lvl::ERROR); }
    virtual void LogCritic(const std::string& msg) { write(msg, log_lvl::CRITICAL); }

    virtual void close() = 0;

    static bool registerLogger(const std::string& name);
    static void unregisterLogger(const std::string& name);

protected:
    Logger(log_lvl level) : allowed_level(level) {}

    virtual void write(const std::string& msg, log_lvl lvl) = 0;

    std::mutex mut;
    log_lvl allowed_level;

    static std::string getCurrentTime();
    const char* level_info(log_lvl lvl);

    inline static std::mutex time_mutex;
    inline static std::mutex registration_mutex;
    inline static std::set<std::string> existed_loggers;
};

class FileLogger : public Logger {
public:
    FileLogger(std::string name, log_lvl level, std::string file_path);
    void close() override;

protected:
    void write(const std::string& msg, log_lvl lvl) override;

private:
    std::string name;
    std::string file_path;
    std::ofstream file_stream;
};

class ConsoleLogger : public Logger {
public:
    ConsoleLogger(std::string name, log_lvl level);
    void close() override;

protected:
    void write(const std::string& msg, log_lvl lvl) override;

private:
    std::string name;
    std::ostream& output = std::cout;
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