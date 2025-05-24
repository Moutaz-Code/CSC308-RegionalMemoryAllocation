#ifndef MEM_H
#define MEM_H

#include <stddef.h>


#if memType == 0 // Malloc
#include "malloc_alloc.c"

#elif memType == 1 // Arena Allocator
#include "arena_alloc.c"

#elif memType == 2 // Pool Allocator
#include "pool_malloc.c"

#else
#error "No memory allocator selected!"

#endif


// char* getAllocationName();
// int mem_init();                 
// void mem_cleanup();             
// void* mem_malloc(size_t size);  
// void  mem_free(void* ptr);      

#endif
