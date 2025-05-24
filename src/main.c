#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
//#define memType 1 // 0 for malloc, 1 for arena, 2 for pool.
#include "memory/mem.h"

#define WORK_DURATION_SECONDS 3
#define ELEMENT_COUNT 10000000 // ~10 million ints

int main() {
    printf("%s\n", getAllocationName()); // Call to get the name of the memory allocator
    printf(">>> Memory Manager Init\n");

    if (!mem_init()) {
        fprintf(stderr, "Failed to initialize memory system.\n");
        return 1;
    }

    printf(">>> Allocating memory\n");
    int* data = (int*)mem_malloc(ELEMENT_COUNT * sizeof(int));
    if (!data) {
        fprintf(stderr, "Allocation failed.\n");
        mem_cleanup();
        return 1;
    }

    printf(">>> Running dummy math for ~%d seconds...\n", WORK_DURATION_SECONDS);
    time_t start = time(NULL);
    int sum = 0;
    while (time(NULL) - start < WORK_DURATION_SECONDS) {
        for (int i = 0; i < ELEMENT_COUNT; ++i) {
            data[i] = i % 100;
            sum += data[i] * 3;
        }
    }

    printf(">>> Computation done. Final sum: %d\n", sum);

    mem_free(data);
    mem_cleanup();
    printf(">>> Memory Manager Cleaned up\n");
    return 0;
}
