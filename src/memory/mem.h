#ifndef MEM_H
#define MEM_H

#include <stddef.h>

char* getAllocationName();
int mem_init();
void mem_cleanup();
void* mem_malloc(size_t size);
void mem_free(void* ptr);

#endif // MEM_H
