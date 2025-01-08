#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>


#include "default_allocator.h"

Allocator* (*allocator_create)(void*, const size_t) = emergency_allocator_create;
void (*allocator_free)(Allocator* const, void* const) = emergency_allocator_free;
void (*allocator_destroy)(Allocator* const) = emergency_allocator_destroy;
void* (*allocator_alloc)(Allocator* const, const size_t) = emergency_allocator_alloc;

int main(int argc, char** argv) {
    (void )argc;

    void* library = dlopen(argv[1], RTLD_LOCAL | RTLD_NOW);
    if (argc > 1 && library != NULL) {
        if (strstr(argv[1], "buddy") != NULL) {
            allocator_create = dlsym(library, "buddy_allocator_create");
            allocator_free = dlsym(library, "buddy_allocator_free");
            allocator_destroy = dlsym(library, "buddy_allocator_destroy");
            allocator_alloc = dlsym(library, "buddy_allocator_alloc");
        }

        if (!allocator_create || !allocator_free || !allocator_destroy || !allocator_alloc) {
            const char msg[] = "warning: failed to find some functions\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            allocator_create = emergency_allocator_create;
            allocator_free = emergency_allocator_free;
            allocator_destroy = emergency_allocator_destroy;
            allocator_alloc = emergency_allocator_alloc;
        }
    } else {
        const char msg[] = "warning: library failed to load, using emergency allocator\n";
        write(STDERR_FILENO, msg, sizeof(msg));
    }

    void* memory_pool = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (memory_pool == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    Allocator* allocator = allocator_create(memory_pool, 4096);
    if (!allocator) {
        perror("allocator_create");
        munmap(memory_pool, 4096);
        return 1;
    }

    int* a = allocator_alloc(allocator, sizeof(int));
    int* b = allocator_alloc(allocator, sizeof(int) * 2);
    int* c = allocator_alloc(allocator, sizeof(int) * 3);

    if (a && b && c) {
        *a = 5;
        b[0] = 1;
        b[1] = 2;
        c[0] = 0;
        c[1] = 1;
        c[2] = 2;

        printf("a: %d, b: %d %d, c: %d %d %d\n", *a, b[0], b[1], c[0], c[1], c[2]);
    } else {
        perror("allocation failed");
    }

    allocator_free(allocator, a);
    allocator_free(allocator, b);
    allocator_free(allocator, c);

    allocator_destroy(allocator);

    munmap(memory_pool, 4096);

    if (library) {
        dlclose(library);
    }

    return 0;
}