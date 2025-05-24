// src/memory/arena_alloc.c
#include "../CRegion/region.h"

static CR_Region* my_region = NULL;

char* getAllocationName() {
    return "CRegion Arena Allocator";
}

int mem_init() {
    my_region = CR_RegionNew();
    return my_region != NULL;
}

void mem_cleanup() {
    CR_RegionRelease(my_region);
}

void* mem_malloc(size_t size) {
    return CR_RegionAlloc(my_region, size);
}

void mem_free(void* ptr) {
    // No-op: CRegion frees everything at once
    (void)ptr;
}
