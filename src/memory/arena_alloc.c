
/*
 * ==========================
 * memory/arena_alloc.c
 * Simple arena (bump) allocator implementation
 * ==========================
 */
#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Default arena capacity; could be parameterized or read from config
#define ARENA_CAPACITY (1024UL * 1024 * 100)  // 100 MB

static char* arena_base    = NULL;
static size_t arena_offset = 0;
static size_t arena_capacity = 0;

static void* arena_alloc_fn(size_t size) {
    // align size to pointer width (optional)
    size_t alignment = sizeof(void*);
    size = (size + alignment - 1) & ~(alignment - 1);

    if (arena_offset + size > arena_capacity) {
        fprintf(stderr, "[arena_alloc] Arena out of memory: requested %zu bytes, available %zu\n",
                size, arena_capacity - arena_offset);
        return NULL;
    }
    void* ptr = arena_base + arena_offset;
    arena_offset += size;
    return ptr;
}

static void arena_free_fn(void* ptr) {
    // No-op: individual frees not supported
    (void)ptr;
}

void mem_init(int choice) {
    (void)choice;
    // Allocate or reset arena
    if (!arena_base) {
        arena_capacity = ARENA_CAPACITY;
        arena_base     = malloc(arena_capacity);
        if (!arena_base) {
            fprintf(stderr, "[arena_init] Failed to allocate arena of %zu bytes\n", arena_capacity);
            exit(EXIT_FAILURE);
        }
    }
    arena_offset = 0;
    mem_malloc = arena_alloc_fn;
    mem_free   = arena_free_fn;
}

void mem_cleanup(void) {
    free(arena_base);
    arena_base = NULL;
    arena_capacity = arena_offset = 0;
}
