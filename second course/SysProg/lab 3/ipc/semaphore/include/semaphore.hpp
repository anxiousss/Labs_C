#pragma once
#include <semaphore.h>
#include <string>
#include <stdexcept>
#include <system_error>
#include <fcntl.h>
#include <logger.hpp>
#include <exceptions.hpp>

class SharedSemaphore {
public:
    explicit SharedSemaphore(const std::string& name);
    ~SharedSemaphore();

    void wait();
    void post();
    void close();

private:
    std::string name_;
    sem_t* sem_;
    std::unique_ptr<Logger> logger_;
};