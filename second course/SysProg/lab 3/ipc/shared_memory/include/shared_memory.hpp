#pragma once

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <stdexcept>
#include <cstring>

class SharedMemory {
public:
    SharedMemory(const std::string& path, int proj_id, size_t size);
    ~SharedMemory();
    void* getData() const;

private:
    int shm_id;
    void* data;
    key_t key;
    size_t size;
};