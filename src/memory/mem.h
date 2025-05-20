/*
 * ==========================
 * memory/mem.h
 * Abstract memory interface: selects and exposes allocator functions
 * ==========================
 */
#ifndef MEM_H
#define MEM_H

#include <stddef.h>

/**
 * Function pointer types for allocation and deallocation
 */
typedef void* (*AllocFn)(size_t size);
typedef void  (*FreeFn)(void* ptr);

/**
 * Global pointers to chosen allocator functions
 */
extern AllocFn mem_malloc;
extern FreeFn  mem_free;

/**
 * Initialize memory subsystem based on user choice:
 * 1 = malloc/free (baseline)
 * 2 = arena allocator
 * 3 = pool allocator
 * 4 = malloc/free override via LD_PRELOAD (pass-through)
 */
void mem_init(int choice);

/**
 * Cleanup memory subsystem (e.g., free arena or pool)
 */
void mem_cleanup(void);

#endif // MEM_H
