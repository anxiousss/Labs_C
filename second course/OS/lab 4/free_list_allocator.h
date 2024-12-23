
#ifndef OS4_FREE_LIST_ALLOCATOR_H
#define OS4_FREE_LIST_ALLOCATOR_H

#include <stdlib.h>
#include <sys/mman.h>

typedef struct Node {
    void* value;
    size_t size;
    struct Node* next;
}Node;

typedef struct {
    Node* head;
} List;

List * free_list_allocator_create(void* const memory, const size_t size);

void free_list_allocator_destroy(List* const list);

void* free_list_allocator_alloc(List* list, ssize_t size);

void free_list_allocator_free(Node* const node, void* memory);

#endif //OS4_FREE_LIST_ALLOCATOR_H
