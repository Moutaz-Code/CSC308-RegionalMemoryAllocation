#define _POSIX_C_SOURCE 199309L // MUST BE FIRST
#include <time.h>               // For timespec, clock_gettime, CLOCK_MONOTONIC

#include "config.h"
#include "graphics/renderer.h"
#include "input/mouse.h"
#include "simulation/entity_manager.h"
#include <stdio.h>


// Helper function to get current time in seconds
static double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // This should now be found
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main(void) {
    printf("Starting application...\n");

    GLFWwindow* window = renderer_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (!window) {
        return 1;
    }

    // Initialize entity manager
    if (!entity_manager_init(NUM_ENTITIES, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        fprintf(stderr, "Failed to initialize entity manager.\n");
        renderer_cleanup();
        return 1;
    }

    double last_time = get_time_in_seconds();

    while (renderer_should_run(window)) {
        double current_time = get_time_in_seconds();
        float dt = (float)(current_time - last_time);
        last_time = current_time;

        renderer_begin_frame();

        double mx = mouse_get_x();
        double my = mouse_get_y();
        // printf("Mouse X: %.2f, Mouse Y: %.2f, dt: %.4f\n", mx, my, dt);

        // Update entities
        entity_manager_update(dt, (float)mx, (float)my);

        // --- Rendering entities will go here in the next step ---

        renderer_end_frame(window);
    }

    entity_manager_cleanup(); // Clean up entities
    renderer_cleanup();
    printf("Application terminated.\n");
    return 0;
}