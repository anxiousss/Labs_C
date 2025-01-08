#ifndef OS4_BUDDY_MEMORY_ALLOCATOR_H
#define OS4_BUDDY_MEMORY_ALLOCATOR_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdalign.h>
#include <stddef.h>
#include <string.h>

#include "default_allocator.h"

typedef struct Block {
    void *start;
    size_t size;
    int is_free;
    struct Block *left;
    struct Block *right;
    struct Block *parent;
    struct Block *next_free;
} Block;

typedef struct BuddyAllocator {
    Allocator allocator;
    Block *root;
    Block *free_tree_nodes;
} BuddyAllocator;

Allocator* buddy_allocator_create(void* memory, const size_t size);
void buddy_allocator_destroy(Allocator *allocator);
void *buddy_allocator_alloc(Allocator *allocator, size_t size);
void buddy_allocator_free(Allocator *allocator, void *ptr);

#endif // OS4_BUDDY_MEMORY_ALLOCATOR_H