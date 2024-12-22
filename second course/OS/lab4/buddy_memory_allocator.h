
#ifndef OS4_BUDDY_MEMORY_ALLOCATOR_H
#define OS4_BUDDY_MEMORY_ALLOCATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdalign.h>
#include <stddef.h>


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
    Block *root;
    void *tree_node_pool;
    size_t tree_node_pool_size;
    Block *free_tree_nodes; // Free list for tree nodes
} BuddyAllocator;

BuddyAllocator *buddy_allocator_create(size_t initial_size, size_t tree_node_pool_size);

Block *allocate_tree_node(BuddyAllocator *allocator);

void deallocate_tree_node(BuddyAllocator *allocator, Block *node);

void insert_block(Block **root, Block *new_block);

Block* find_min(Block* node);

Block* find_previous_block(Block* root, Block* block);

Block* find_next_block(Block* root, Block* block);

Block* remove_block(Block* root, Block* block);

Block *find_smallest_free_block(Block *root, size_t size);

void merge_blocks(BuddyAllocator *allocator, Block *block);

Block* find_block(Block* root, void* ptr);

void *buddy_allocator_alloc(BuddyAllocator *allocator, size_t size);

void buddy_allocator_free(BuddyAllocator *allocator, void *ptr);

void buddy_allocator_destroy(BuddyAllocator* allocator);


#endif //OS4_BUDDY_MEMORY_ALLOCATOR_H
