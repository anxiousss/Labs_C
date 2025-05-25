#pragma once

#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <stdexcept>
#include <cstring>

class MessageQueue {
public:
    MessageQueue(const std::string& path, int proj_id);
    ~MessageQueue();
    void send(const void* msg, size_t size, long mtype = 1);
    void receive(void* msg, size_t size, long mtype = 0);

private:
    int msg_id;
    key_t key;
};