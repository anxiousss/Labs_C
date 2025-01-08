#include "buddy_memory_allocator.h"

Allocator* buddy_allocator_create(void* memory, const size_t size) {
    BuddyAllocator *allocator = mmap(NULL, sizeof(BuddyAllocator), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (allocator == MAP_FAILED) {
        return NULL;
    }

    void* memory_pool;
    if (memory == NULL) {
        memory_pool = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (memory_pool == MAP_FAILED) {
            munmap(allocator, sizeof(BuddyAllocator));
            return NULL;
        }
    } else {
        memory_pool = memory;
    }

    // Initialize the root block
    Block *initial_block = (Block *)memory_pool;
    initial_block->start = (void *)((uintptr_t)memory_pool + sizeof(Block));
    initial_block->size = size - sizeof(Block);
    initial_block->is_free = 1;
    initial_block->left = NULL;
    initial_block->right = NULL;
    initial_block->parent = NULL;
    initial_block->next_free = NULL;

    allocator->root = initial_block;
    allocator->free_tree_nodes = NULL;

    return (Allocator*) allocator;
}

Block *allocate_tree_node(BuddyAllocator *allocator) {
    if (allocator->free_tree_nodes == NULL) {
        return NULL;
    }
    Block *node = allocator->free_tree_nodes;
    allocator->free_tree_nodes = node->next_free;
    return node;
}

void deallocate_tree_node(BuddyAllocator *allocator, Block *node) {
    node->next_free = allocator->free_tree_nodes;
    allocator->free_tree_nodes = node;
}

void insert_block(Block **root, Block *new_block) {
    if (*root == NULL) {
        *root = new_block;
        return;
    }
    if ((uintptr_t)new_block->start < (uintptr_t)(*root)->start) {
        insert_block(&(*root)->left, new_block);
    } else {
        insert_block(&(*root)->right, new_block);
    }
}

Block* find_min(Block* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Block* find_previous_block(Block* root, Block* block) {
    Block* prev = NULL;
    while (root != NULL) {
        if ((uintptr_t)block->start > (uintptr_t)root->start) {
            prev = root;
            root = root->right;
        } else {
            root = root->left;
        }
    }
    return prev;
}

Block* find_next_block(Block* root, Block* block) {
    Block* next = NULL;
    while (root != NULL) {
        if ((uintptr_t)block->start < (uintptr_t)root->start) {
            next = root;
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return next;
}

Block* remove_block(Block* root, Block* block) {
    if (root == NULL) {
        return root;
    }
    if ((uintptr_t)block->start < (uintptr_t)root->start) {
        root->left = remove_block(root->left, block);
    } else if ((uintptr_t)block->start > (uintptr_t)root->start) {
        root->right = remove_block(root->right, block);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(block);
            return NULL;
        }
        if (root->right == NULL) {
            Block* temp = root->left;
            free(root);
            return temp;
        }
        if (root->left == NULL) {
            Block* temp = root->right;
            free(root);
            return temp;
        }
        Block* temp = find_min(root->right);
        root->start = temp->start;
        root->size = temp->size;
        root->right = remove_block(root->right, temp);
    }
    return root;
}

Block *find_smallest_free_block(Block *root, size_t size) {
    if (root == NULL) {
        return NULL;
    }
    if (root->is_free && root->size >= size) {
        return root;
    }
    Block *left = find_smallest_free_block(root->left, size);
    if (left != NULL) {
        return left;
    }
    return find_smallest_free_block(root->right, size);
}

void merge_blocks(BuddyAllocator *allocator, Block *block) {
    if (block == NULL || !block->is_free) {
        return;
    }

    Block *prev = find_previous_block(allocator->root, block);
    if (prev && prev->is_free) {
        prev->size += block->size;
        if (block->right)
            block->right->parent = prev;
        remove_block(allocator->root, block);
        deallocate_tree_node(allocator, block);
        block = prev;
    }

    Block *next = find_next_block(allocator->root, block);
    if (next && next->is_free) {
        block->size += next->size;
        if (next->right)
            next->right->parent = block;
        remove_block(allocator->root, next);
        deallocate_tree_node(allocator, next);
    }
}

Block* find_block(Block* root, void* ptr) {
    if (root == NULL)
        return NULL;
    uintptr_t ptr_val = (uintptr_t)ptr;
    uintptr_t block_start = (uintptr_t)root->start;
    uintptr_t block_end = block_start + root->size;

    if (ptr_val >= block_start && ptr_val < block_end)
        return root;
    else if (ptr_val < block_start && root->left != NULL)
        return find_block(root->left, ptr);
    else if (ptr_val >= block_end && root->right != NULL)
        return find_block(root->right, ptr);
    else
        return NULL;
}

void *buddy_allocator_alloc(Allocator *allocator, size_t size) {
    BuddyAllocator* buddyAllocator = (BuddyAllocator*)allocator;
    if (!buddyAllocator || size == 0)
        return NULL;

    size_t aligned_size = ((size + __alignof__(max_align_t) - 1) / __alignof__(max_align_t)) * __alignof__(max_align_t);

    Block *block = find_smallest_free_block(buddyAllocator->root, aligned_size);
    if (!block)
        return NULL;

    if (block->size == aligned_size) {
        block->is_free = 0;
        return block->start;
    } else if (block->size > aligned_size) {
        Block *new_block = allocate_tree_node(buddyAllocator);
        if (!new_block)
            return NULL;

        new_block->start = (void *)((uintptr_t)block->start + aligned_size);
        new_block->size = block->size - aligned_size;
        new_block->is_free = 1;
        new_block->left = NULL;
        new_block->right = NULL;
        new_block->parent = block;
        new_block->next_free = NULL;

        insert_block(&buddyAllocator->root, new_block);

        block->size = aligned_size;
        block->is_free = 0;

        return block->start;
    } else {
        block->is_free = 0;
        return block->start;
    }
}

void buddy_allocator_free(Allocator *allocator, void *ptr) {
    BuddyAllocator* buddyAllocator = (BuddyAllocator*)allocator;

    if (!buddyAllocator || !ptr)
        return;

    Block *block = find_block(buddyAllocator->root, ptr);
    if (!block)
        return;

    block->is_free = 1;
    merge_blocks(buddyAllocator, block);

    if (block->is_free && block->left == NULL && block->right == NULL) {
        if (block->start != buddyAllocator->root->start) {
            munmap(block->start, block->size);
        }
        remove_block(buddyAllocator->root, block);
        deallocate_tree_node(buddyAllocator, block);
    }
}

void buddy_allocator_destroy(Allocator* allocator) {
    BuddyAllocator* buddyAllocator = (BuddyAllocator*)allocator;
    if (buddyAllocator == NULL) {
        return;
    }

    Block* current = buddyAllocator->root;
    while (current != NULL) {
        Block* next = current->right;
        if (current != buddyAllocator->root) {
            munmap(current->start, current->size);
        }
        current = next;
    }

    munmap(buddyAllocator, sizeof(BuddyAllocator));
}