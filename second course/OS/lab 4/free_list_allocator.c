#include "free_list_allocator.h"

Allocator* free_list_allocator_create(void* const memory, const size_t size) {
    List* list = mmap(NULL, sizeof(List), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (list == MAP_FAILED)
        return NULL;

    list->head->value = memory;
    list->head->size = size;
    return list;
};

void free_list_allocator_free(Allocator * const allocator, void* memory) {
    List* list = (List*)allocator;
    Node* cur = list->head;
    while (cur->next->value != memory) {
        cur = cur->next;
    }
    munmap(memory, cur->next->size);
}

void* free_list_allocator_alloc(Allocator* allocator, ssize_t size) {
    List* list = (List*)allocator;
    Node* cur = list->head;
    while (cur->next != NULL)
        cur = cur->next;

    cur->next = mmap(NULL, sizeof(Node), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (cur->next == MAP_FAILED) {
        return NULL;
    }
    cur->next->value = mmap(NULL, size, PROT_WRITE| PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (cur->next->value == MAP_FAILED) {
        return NULL;
    }
    cur->next->size = size;
    return cur->next;
}

void free_list_allocator_destroy(Allocator * const allocator) {
    List* list = (List*)allocator;
    Node* cur = list->head;
    while (cur->next != NULL) {
        Node* tmp = cur;
        cur = cur->next;
        munmap(tmp->value, tmp->size);
        munmap(tmp, sizeof(Node));
    }
}

