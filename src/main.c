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
        renderer_cleanup(); // This will call glfwTerminate()
        return 1;
    }

    double last_time = get_time_in_seconds();

    while (renderer_should_run(window)) {
        double current_time = get_time_in_seconds();
        float dt = (float)(current_time - last_time);
        last_time = current_time;

        renderer_begin_frame();
        
        double mx_raw = mouse_get_x(); // Get raw mouse x
        double my_raw = mouse_get_y(); // Get raw mouse y
        
        float target_x = (float)mx_raw;
        float target_y = (float)my_raw; 

        // VERY IMPORTANT: Print what's actually being sent
        printf("Main: Raw Mouse(%.2f, %.2f) -> Target(%.2f, %.2f)\n", mx_raw, my_raw, target_x, target_y);

        // Update entities
        entity_manager_update(dt, target_x, target_y);

        // Get entity data for rendering
        const Entity* entities_to_draw = entity_manager_get_entities();
        size_t num_entities_to_draw = entity_manager_get_count();
        
        // DEBUG: Print the number of entities and dt
        printf("Main Loop: Drawing %zu entities. dt = %f\n", num_entities_to_draw, dt); 
        
        renderer_draw_entities(entities_to_draw, num_entities_to_draw);

        renderer_end_frame(window);
    }

    entity_manager_cleanup();
    renderer_cleanup(); // This now handles glfwTerminate()
    printf("Application terminated.\n");
    return 0;
}