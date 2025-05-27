#pragma once
#include <string>
#include <vector>
#include <memory>
#include <logger.hpp>
#include <shared_memory.hpp>
#include <exceptions.hpp>

class CompilerSubprocess {
public:
    explicit CompilerSubprocess(const std::string& queue_name);
    void run();

private:
    std::string queue_name_;
    std::unique_ptr<Logger> logger_;

    void process_task(const SharedMemoryTask& task);
};