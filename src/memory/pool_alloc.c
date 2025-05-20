
/*
 * ==========================
 * memory/pool_alloc.c
 * Simple fixed-size pool allocator implementation
 * ==========================
 */
#include "mem.h"
#include <stdlib.h>
#include <stdio.h>

// Pool parameters
#define POOL_BLOCK_SIZE  256   // bytes per block
#define POOL_BLOCK_COUNT 1024  // number of blocks

typedef struct PoolNode {
    struct PoolNode* next;
} PoolNode;

static PoolNode* free_list = NULL;
static void* pool_memory = NULL;

static void pool_init_memory(void) {
    pool_memory = malloc(POOL_BLOCK_SIZE * POOL_BLOCK_COUNT);
    if (!pool_memory) {
        fprintf(stderr, "[pool_init] Failed to allocate pool memory\n");
        exit(EXIT_FAILURE);
    }

    // Build free list
    free_list = NULL;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; ++i) {
        PoolNode* node = (PoolNode*)((char*)pool_memory + i * POOL_BLOCK_SIZE);
        node->next = free_list;
        free_list = node;
    }
}

static void* pool_alloc_fn(size_t size) {
    if (size > POOL_BLOCK_SIZE) {
        // fallback to malloc for large requests
        return malloc(size);
    }
    if (!free_list) {
        fprintf(stderr, "[pool_alloc] Pool exhausted\n");
        return NULL;
    }
    PoolNode* node = free_list;
    free_list = free_list->next;
    return node;
}

static void pool_free_fn(void* ptr) {
    if (ptr >= pool_memory && ptr < (char*)pool_memory + POOL_BLOCK_SIZE * POOL_BLOCK_COUNT) {
        // return to pool
        PoolNode* node = (PoolNode*)ptr;
        node->next = free_list;
        free_list = node;
    } else {
        // not in pool, free normally
        free(ptr);
    }
}

void mem_init(int choice) {
    (void)choice;
    if (!pool_memory) {
        pool_init_memory();
    }
    mem_malloc = pool_alloc_fn;
    mem_free   = pool_free_fn;
}

void mem_cleanup(void) {
    free(pool_memory);
    pool_memory = NULL;
    free_list = NULL;
}
