
/*
 * ==========================
 * memory/mem_loader.c
 * Reads user choice and initializes the memory subsystem
 * ==========================
 */
#include "mem.h"
#include <stdio.h>

void mem_init(int choice);
void mem_cleanup(void);

AllocFn mem_malloc = NULL;
FreeFn  mem_free   = NULL;

void mem_loader(int argc, char** argv) {
    int choice = 1; // default to malloc/free
    if (argc >= 2) {
        choice = atoi(argv[1]);
    } else {
        printf("Select memory strategy:\n");
        printf("1) malloc/free\n2) arena\n3) pool\n4) LD_PRELOAD (pass-through)\n");
        scanf("%d", &choice);
    }

    switch (choice) {
        case 2:
            printf("[Mem] Using Arena Allocator\n");
            mem_init(2);
            break;
        case 3:
            printf("[Mem] Using Pool Allocator\n");
            mem_init(3);
            break;
        case 4:
            printf("[Mem] Using LD_PRELOAD override\n");
            mem_init(1);
            break;
        case 1:
        default:
            printf("[Mem] Using system malloc/free\n");
            mem_init(1);
            break;
    }
}

// Alias mem_loader to mem_init for consistency
void mem_init(int choice) __attribute__((alias("mem_loader")));

void mem_cleanup(void) {
    // Call specific cleanup based on which init was used
    // Note: mem_cleanup will be overridden by each module's cleanup
}
