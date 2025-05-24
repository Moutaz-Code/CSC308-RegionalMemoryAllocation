#include "mem.h"
#include <stdlib.h>

char* getAllocationName() {
    return "Malloc Allocation"; 
}

int mem_init() {
    // No special init needed for malloc.
    return 1;
}

void mem_cleanup() {
    // Nothing to cleanup for malloc.
}

void* mem_malloc(size_t size) {
    return malloc(size);
}

void mem_free(void* ptr) {
    free(ptr);
}
