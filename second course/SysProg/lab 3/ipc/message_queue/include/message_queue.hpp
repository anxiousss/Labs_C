#pragma once
#include <sys/msg.h>
#include <string>
#include <stdexcept>
#include <system_error>
#include <cstring>
#include <logger.hpp>
#include <exceptions.hpp>


struct MessageBuffer {
    long mtype;
    char mtext[4096];
};

class MessageQueue {
public:
    explicit MessageQueue(key_t key);
    ~MessageQueue();

    void send(const std::string& message, long type = 1);
    std::string receive(long type = 0);
    void close();

private:
    key_t key_;
    int msqid_;
    std::unique_ptr<Logger> logger_;
};