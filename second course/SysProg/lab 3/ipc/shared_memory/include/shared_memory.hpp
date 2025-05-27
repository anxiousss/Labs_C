#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <system_error>
#include <logger.hpp>
#include <exceptions.hpp>

struct SharedMemoryTask {
    std::string filename;
    std::string content;
    std::string extension;
    std::string result;
    bool success = false;
};

class SharedMemoryManager {
public:
    explicit SharedMemoryManager(const std::string& name, size_t size = 4096);
    ~SharedMemoryManager();

    SharedMemoryTask receive();
    void send(const SharedMemoryTask& task);
    void close();

private:
    std::string name_;
    size_t size_;
    int shm_fd_;
    void* ptr_;
    std::unique_ptr<Logger> logger_;
};