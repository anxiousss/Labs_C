#include "solution.h"

int semid;

int main() {
    if ((semid = semget(KEY, N, 0666)) != -1) {
        semctl(semid, 0, IPC_RMID);
    }

    semid = semget(KEY, N, IPC_CREAT | 0666);
    if (semid == -1) {
        printf("Sem error\n");
        return Sem_error;
    }

    unsigned short values[N];
    for (int i = 0; i < N; ++i) {
        values[i] = 1;
    }
    semun arg;
    arg.array = values;

    if (semctl(semid, 0, SETALL, arg) == -1) {
        semctl(semid, 0, IPC_RMID);
        printf("Sem error");
        return Sem_error;
    }

    pthread_t threads[N];

    int identifiers[N];
    for (int i = 0; i < N; ++i) {
        identifiers[i] = i;
        if (pthread_create(&threads[i], NULL, process, identifiers + i)) {
            printf("Pthread error\n");
            for (int j = 0; j < i; ++j) {
                pthread_join(threads[j], NULL);
            }

            semctl(semid, 0, IPC_RMID);
            return Pthread_error;
        }
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    semctl(semid, 0, IPC_RMID);
    return 0;
}
