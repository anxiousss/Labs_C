#include "solution.h"


int main(int argc, char* argv[]) {
    int N;
    int err = get_options(argc, argv, &N);
    if (err) {
        printf("Invalid options\n");
        return err;
    }

    RoomState state;
    err = room_init(&state, N);
    switch (err) {
        case Mutex_error:
            printf("Mutex error\n");
            return err;
        case Cond_error:
            printf("Cond error\n");\
            return err;
    }

    pthread_t threads[10];

    int thread_count = 0;

    for (int i = 0; i < 5; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->state = &state;
        args->id = i + 1;

        if (create_thread(&threads[thread_count], woman_thread, args) == 0) {
            thread_count++;
        }

        args = malloc(sizeof(ThreadArgs));
        args->state = &state;
        args->id = i + 6;

        if (create_thread(&threads[thread_count], man_thread, args) == 0) {
            thread_count++;
        }
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    room_destroy(&state);
    pthread_mutex_destroy(&print_mutex);
    return 0;
}
