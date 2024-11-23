#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

int _Atomic inside;

void* rand_point(void * radius) {
    double* r = (double *)radius;
    double x = (double)rand() / RAND_MAX * (2 * *r) - *r;
    double  y = (double)rand() / RAND_MAX * (2 * *r) - *r;
    if (x*x + y*y <= (*r)*(*r)) {
        atomic_fetch_add(&inside, 1);
    }
    return NULL;
}

double circle_area(double r, int N) {

    pthread_t threads[N];

    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], NULL, rand_point, (void*)&r);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    double area_square = 4 * r * r;
    double area_circle = ((double)inside / N) * area_square;

    return area_circle;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    int N = atoi(argv[1]);
    double r = 2.0;
    printf("thread number: %d circle area of r = %lf = %lf", N, r, circle_area(r, N));
    return 0;
}
