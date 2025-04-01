#include "solution.h"

int sem_op(int sem_num, int op_val) {
    struct sembuf op = {sem_num, op_val, IPC_NOWAIT};
    if (semop(semid, &op, 1) == -1) {
        return Sem_error;
    }
    return 0;
}


void* process(void* arg) {
    int id = *((int*)arg);

    for (int i = 0; i < 100; ++i) {
        int left = id;
        int right = (id + 1) % N;
        int first = min(left, right);
        int second = max(left, right);

        struct sembuf ops[2] = {
                {first, -1, 0},
                {second, -1, 0}
        };

        semop(semid, ops, 2);
        printf("Philosopher %d eating (forks %d & %d)\n", id, first, second);
        sleep(rand() % 10);

        struct sembuf release_ops[2] = {
                {first, 1, 0},
                {second, 1, 0}
        };
        semop(semid, release_ops, 2);

        printf("Philosopher %d thinking\n", id);
        sleep(rand() % 10);
    }
    return NULL;
}
