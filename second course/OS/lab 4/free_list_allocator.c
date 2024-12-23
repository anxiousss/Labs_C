#include "free_list_allocator.h"

List * free_list_allocator_create(void* const memory, const size_t size) {
    List* list = mmap(NULL, sizeof(List), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (list == MAP_FAILED)
        return NULL;

    list->head->value = memory;
    list->head->size = size;
    return list;
};

void free_list_allocator_free(Node* const node, void* memory) {
    munmap(memory, node->size);
}

void* free_list_allocator_alloc(List* list, ssize_t size) {
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

void free_list_allocator_destroy(List* const list) {
    Node* cur = list->head;
    while (cur->next != NULL) {
        Node* tmp = cur;
        free_list_allocator_free(tmp, tmp->value);
        munmap(tmp, tmp->size);
        cur = cur->next;
    }
}

