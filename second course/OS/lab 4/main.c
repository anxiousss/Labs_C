#include "buddy_memory_allocator.h"
#include "free_list_allocator.h"

typedef struct {
  void* memory;
  size_t size;
} Allocator;

Allocator* allocator_create(void *const memory, const size_t size){
    Allocator* allocator = mmap(NULL, sizeof(Allocator), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (allocator == MAP_FAILED)
        return NULL;

    allocator->memory = memory;
    allocator->size = size;
    return allocator;
}

void allocator_free(Allocator *const allocator, void *const memory) {
    munmap(memory, allocator->size);
}

void allocator_destroy(Allocator *const allocator) {
    allocator_free(allocator, allocator->memory);
    munmap(allocator, sizeof(Allocator));
}

void* allocator_alloc(Allocator *const allocator, const size_t size) {
    if (!allocator->memory) {
        allocator->memory = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (!allocator->memory) {
            allocator_destroy(allocator);
            return NULL;
        }
    }
    return allocator->memory;
}


int main(void) {

    return 0;
}
