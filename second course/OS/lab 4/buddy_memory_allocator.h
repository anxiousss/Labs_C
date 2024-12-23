
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
    struct Block *next_free; // For free list in tree node pool
} Block;

typedef struct BuddyAllocator {
    Allocator allocator;
    Block *root;
    Block *free_tree_nodes; // Free list for tree nodes
} BuddyAllocator;

Allocator* buddy_allocator_create(void* memory, const size_t size);

Block *allocate_tree_node(BuddyAllocator *allocator);

void deallocate_tree_node(Block *node);

void insert_block(Block **root, Block *new_block);

Block* find_min(Block* node);

Block* find_previous_block(Block* root, Block* block);

Block* find_next_block(Block* root, Block* block);

Block* remove_block(Block* root, Block* block);

Block *find_smallest_free_block(Block *root, size_t size);

void merge_blocks(BuddyAllocator *allocator, Block *block);

Block* find_block(Block* root, void* ptr);

void *buddy_allocator_alloc(Allocator *allocator, size_t size);

void buddy_allocator_free(Allocator *allocator, void *ptr);

void buddy_allocator_destroy(Allocator * allocator);


#endif //OS4_BUDDY_MEMORY_ALLOCATOR_H
