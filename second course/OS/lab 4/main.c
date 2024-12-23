#include <dlfcn.h>

#include "buddy_memory_allocator.h"
#include "free_list_allocator.h"
#include "default_allocator.h"



Allocator* (*allocator_create)(void*, const size_t) = emergency_allocator_create;

void (*allocator_free)(Allocator* const, void* const) = emergency_allocator_free;

void (*allocator_destroy)(Allocator* const) = emergency_allocator_destroy;

void* (*allocator_alloc)(Allocator* const, const size_t) = emergency_allocator_alloc;




int main(int argc, char** argv) {
    (void )argc;

    void* library = dlopen(argv[1], RTLD_LOCAL | RTLD_NOW);
    if (argc > 1 && library != NULL) {
        if (strstr(argv[1], "free_list") != NULL) {
            allocator_create = dlsym(library, "free_list_allocator_create");
            allocator_free = dlsym(library, "free_list_allocator_free");
            allocator_destroy = dlsym(library, "free_list_allocator_destroy");
            allocator_alloc = dlsym(library, "free_list_allocator_alloc");

        } else if (strstr(argv[1], "buddy")) {
            allocator_create= dlsym(library, "buddy_allocator_create");
            allocator_free = dlsym(library, "buddy_allocator_free");
            allocator_destroy = dlsym(library, "buddy_allocator_destroy");
            allocator_alloc = dlsym(library, "buddy_allocator_alloc");
        }

        if (!allocator_create && !allocator_free && !allocator_destroy && !allocator_alloc) {
            const char msg[] = "warning failed to find some function";
            write(STDERR_FILENO, msg, sizeof(msg));
            allocator_create = emergency_allocator_create;
            allocator_free = emergency_allocator_free;
            allocator_destroy = emergency_allocator_destroy;
            allocator_alloc = emergency_allocator_alloc;
        }
    } else {
        const char msg[] = "warning: library failed to load, trying standard implemntations\n";
        write(STDERR_FILENO, msg, sizeof(msg));
    }

    if (library) {
        dlclose(library);
    }

    int a = 5;
    Allocator* allocator = allocator_create(NULL, sizeof(int));
    int* b = allocator_alloc(allocator, sizeof(int));
    allocator_free(allocator, b);
    allocator_destroy(allocator);
    return 0;
}
