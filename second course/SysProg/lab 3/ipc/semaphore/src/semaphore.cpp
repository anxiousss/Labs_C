#include "semaphore.hpp"

SharedSemaphore::SharedSemaphore(const std::string& name)
        : name_("/" + name), sem_(nullptr), logger_(LoggerBuilder("Semaphore").set_console().build()) {
    sem_ = sem_open(name_.c_str(), O_CREAT | O_RDWR, 0666, 1);
    if (sem_ == SEM_FAILED) {
        throw std::system_error(errno, std::generic_category(), "sem_open failed");
    }
    logger_->LogInfo("Created semaphore: " + name_);
}

SharedSemaphore::~SharedSemaphore() {
    close();
    sem_unlink(name_.c_str());
}

void SharedSemaphore::close() {
    if (sem_ != nullptr) {
        if (sem_close(sem_) == -1) {
            logger_->LogError("sem_close failed");
        }
        sem_ = nullptr;
    }
}

void SharedSemaphore::wait() {
    if (sem_ == SEM_FAILED) {
        throw std::runtime_error("Semaphore not initialized");
    }

    if (sem_wait(sem_) == -1) {
        throw std::system_error(errno, std::generic_category(), "sem_wait failed");
    }
}

void SharedSemaphore::post() {
    if (sem_ == SEM_FAILED) {
        throw std::runtime_error("Semaphore not initialized");
    }

    if (sem_post(sem_) == -1) {
        throw std::system_error(errno, std::generic_category(), "sem_post failed");
    }
}