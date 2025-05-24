#include "mem.h"
#include <stdlib.h>

char* getAllocationName() {
    return "Arena Allocation"; 
}

int mem_init() {
    
    return 1;
}

void mem_cleanup() {
    
}

void* mem_malloc(size_t size) {
    return malloc(size);
}

void mem_free(void* ptr) {
    free(ptr);
}
