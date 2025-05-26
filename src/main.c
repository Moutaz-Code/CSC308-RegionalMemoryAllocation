// filepath: src/main.c
#include "config.h"
#include "graphics/renderer.h"
#include <stdio.h> // For printf

int main(void) {
    printf("Starting application...\n");

    GLFWwindow* window = renderer_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (!window) {
        return 1;
    }

    while (renderer_should_run(window)) {
        renderer_begin_frame();
        // --- Game logic and other rendering will go here ---
        renderer_end_frame(window);
    }

    renderer_cleanup();
    printf("Application terminated.\n");
    return 0;
}