#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>

atomic_int inside = 0; // Initialize inside to zero

// Structure to pass data to each thread
typedef struct {
    double radius;
    int iterations;
    atomic_int *inside_ptr;
} thread_data;

void* rand_point(void* data) {
    thread_data *td = (thread_data *)data;
    double r = td->radius;
    int iterations = td->iterations;
    for (int i = 0; i < iterations; ++i) {
        double x = (double)rand() / RAND_MAX * (2 * r) - r;
        double y = (double)rand() / RAND_MAX * (2 * r) - r;
        if (x*x + y*y <= r*r) {
            atomic_fetch_add(td->inside_ptr, 1); // Corrected line
        }
    }
    return NULL;
}

double circle_area(double r, int total_iterations, int num_threads) {
    pthread_t threads[num_threads];
    thread_data data[num_threads];

    // Calculate iterations per thread
    int base_iterations = total_iterations / num_threads;
    int extra_iterations = total_iterations % num_threads;

    for (int i = 0; i < num_threads; ++i) {
        data[i].radius = r;
        data[i].iterations = base_iterations + (i < extra_iterations ? 1 : 0);
        data[i].inside_ptr = &inside;
        // Create threads
        if (pthread_create(&threads[i], NULL, rand_point, &data[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    double area_square = 4 * r * r;
    double area_circle = ((double)inside / total_iterations) * area_square;

    return area_circle;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_threads> <total_iterations>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int num_threads = atoi(argv[1]);
    int total_iterations = atoi(argv[2]);
    double r = 1.0;
    double area = circle_area(r, total_iterations, num_threads);

    char buf[256];
    snprintf(buf, sizeof(buf), "thread number: %d, total iterations: %d, circle area of r = %lf = %lf\n",
             num_threads, total_iterations, r, area);
    write(STDOUT_FILENO, buf, strlen(buf));

    return 0;
}
