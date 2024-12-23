
#ifndef OS4_FREE_LIST_ALLOCATOR_H
#define OS4_FREE_LIST_ALLOCATOR_H

#include "default_allocator.h"

typedef struct Node {
    void* value;
    size_t size;
    struct Node* next;
}Node;

typedef struct {
    Allocator allocator;
    Node* head;
} List;

Allocator* free_list_allocator_create(void* const memory, const size_t size);

void free_list_allocator_destroy(Allocator* const allocator);

void* free_list_allocator_alloc(Allocator * allocator, ssize_t size);

void free_list_allocator_free(Allocator* const allocator, void* memory);

#endif //OS4_FREE_LIST_ALLOCATOR_H
