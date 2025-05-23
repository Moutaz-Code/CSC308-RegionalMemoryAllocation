//This file is to be called by the main program to load the needed memory module (arena_alloc.c, pool_malloc.c, or malloc.c).

#include <stdio.h>
#include <stdlib.h>

// Function pointer types for memory operations
typedef void* (*mem_alloc_func)(size_t);
typedef void  (*mem_free_func)(void*);

// Declarations for memory modules
void* arena_alloc_malloc(size_t size);
void  arena_alloc_free(void* ptr);

void* pool_malloc_malloc(size_t size);
void  pool_malloc_free(void* ptr);

void* std_malloc(size_t size);
void  std_free(void* ptr);

// Struct to hold chosen memory functions
typedef struct {
    mem_alloc_func alloc;
    mem_free_func  free;
} mem_loader_t;

// Loader function to select memory module
void mem_loader_init(mem_loader_t* loader, int choice) {
    switch (choice) {
        case 1: // Standard malloc
            loader->alloc = std_malloc;
            loader->free  = std_free;
            break;
        case 2: // Arena allocator
            loader->alloc = arena_alloc_malloc;
            loader->free  = arena_alloc_free;
            break;
        case 3: // Pool allocator
            loader->alloc = pool_malloc_malloc;
            loader->free  = pool_malloc_free;
            break;
        default:
            fprintf(stderr, "Invalid memory module choice. Defaulting to malloc.\n");
            loader->alloc = std_malloc;
            loader->free  = std_free;
    }
}

// Standard malloc/free wrappers
void* std_malloc(size_t size) {
    return malloc(size);
}

void std_free(void* ptr) {
    free(ptr);
}