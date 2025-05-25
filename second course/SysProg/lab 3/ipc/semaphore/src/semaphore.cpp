#include "semaphore.hpp"

Semaphore::Semaphore(const std::string& path, int proj_id, int initial) {
    key = ftok(path.c_str(), proj_id);
    if (key == -1) throw std::runtime_error("ftok failed for Semaphore");

    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) throw std::runtime_error("semget failed");

    union semun arg;
    arg.val = initial;
    if (semctl(sem_id, 0, SETVAL, arg) == -1)
        throw std::runtime_error("semctl failed");
}

Semaphore::~Semaphore() {
    // Семафоры обычно не удаляются автоматически
}

void Semaphore::wait() {
    struct sembuf op = {0, -1, 0};
    if (semop(sem_id, &op, 1) == -1)
        throw std::runtime_error("semop wait failed");
}

void Semaphore::post() {
    struct sembuf op = {0, 1, 0};
    if (semop(sem_id, &op, 1) == -1)
        throw std::runtime_error("semop post failed");
}