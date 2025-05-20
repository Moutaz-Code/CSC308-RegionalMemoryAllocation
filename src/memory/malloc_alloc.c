
/*
 * ==========================
 * memory/malloc_alloc.c
 * Default malloc/free allocator implementation
 * ==========================
 */
#include "mem.h"
#include <stdlib.h>
#include <stdio.h>

static void* malloc_alloc(size_t size) {
    void* p = malloc(size);
    if (!p) {
        fprintf(stderr, "[malloc_alloc] Out of memory allocating %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

static void free_alloc(void* ptr) {
    free(ptr);
}

/** Exported function pointers */
AllocFn mem_malloc = malloc_alloc;
FreeFn  mem_free   = free_alloc;

void mem_init(int choice) {
    (void)choice;  // ignore, use default
    mem_malloc = malloc_alloc;
    mem_free   = free_alloc;
}

void mem_cleanup(void) {
    // No cleanup needed for malloc/free
}

