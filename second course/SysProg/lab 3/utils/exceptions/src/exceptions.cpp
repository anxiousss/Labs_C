#include "exceptions.hpp"

ExceptionBase::ExceptionBase(std::string message, log_lvl level, std::string source)
        : std::runtime_error(std::move(message)), source_(std::move(source)), level_(level) {}

const std::string& ExceptionBase::getSource() const {
    return source_;
}

log_lvl ExceptionBase::getLogLevel() const {
    return level_;
}

void ExceptionBase::log(std::unique_ptr<Logger>& logger) const {
    switch (level_) {
        case log_lvl::DEBUG:
            logger->LogDebug("Exception in " + source_ + ": " + what());
            break;
        case log_lvl::INFO:
            logger->LogInfo("Exception in " + source_ + ": " + what());
            break;
        case log_lvl::WARNING:
            logger->LogWarning("Exception in " + source_ + ": " + what());
            break;
        case log_lvl::ERROR:
            logger->LogError("Exception in " + source_ + ": " + what());
            break;
        case log_lvl::CRITICAL:
            logger->LogCritic("Exception in " + source_ + ": " + what());
            break;
    }
}