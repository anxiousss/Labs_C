#pragma once
#include <string>
#include <stdexcept>
#include <logger.hpp>

class ExceptionBase : public std::runtime_error {
public:
    explicit ExceptionBase(std::string message, log_lvl level, std::string source);

    const std::string& getSource() const;
    log_lvl getLogLevel() const;
    void log(std::unique_ptr<Logger>& logger) const;

private:
    std::string source_;
    log_lvl level_;
};