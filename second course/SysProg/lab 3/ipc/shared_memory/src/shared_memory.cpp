#include "shared_memory.hpp"

SharedMemory::SharedMemory(const std::string& path, int proj_id, size_t size) : size(size) {
    key = ftok(path.c_str(), proj_id);
    if (key == -1) throw std::runtime_error("ftok failed for SharedMemory");

    shm_id = shmget(key, size, IPC_CREAT | 0666);
    if (shm_id == -1) throw std::runtime_error("shmget failed");

    data = shmat(shm_id, nullptr, 0);
    if (data == (void*)-1) throw std::runtime_error("shmat failed");
}

SharedMemory::~SharedMemory() {
    shmdt(data);
}

void* SharedMemory::getData() const {
    return data;
}