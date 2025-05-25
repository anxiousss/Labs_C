#pragma once

#include <sys/ipc.h>
#include <sys/sem.h>
#include <string>
#include <stdexcept>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

class Semaphore {
public:
    Semaphore(const std::string& path, int proj_id, int initial = 0);
    ~Semaphore();
    void wait();
    void post();

private:
    int sem_id;
    key_t key;
};