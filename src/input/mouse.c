#include "mouse.h"
#include <stdio.h> // For printf in callbacks (optional debugging)

// Static variables to store mouse state
static double current_mouse_x = 0.0;
static double current_mouse_y = 0.0;

// GLFW callback function for cursor position
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // xpos and ypos are screen coordinates.
    // GLFW's origin (0,0) is top-left. OpenGL's typical origin is bottom-left.
    // For now, we'll store them as is. We can transform them later if needed.
    current_mouse_x = xpos;
    current_mouse_y = ypos;
    //printf("Mouse position: %f, %f\n", xpos, ypos); // Optional: for debugging
}

void mouse_init(GLFWwindow* window) {
    if (!window) {
        fprintf(stderr, "ERROR (Mouse): GLFW window is NULL, cannot initialize mouse input.\n");
        return;
    }
    // Set the cursor position callback
    glfwSetCursorPosCallback(window, cursor_position_callback);
    printf("Mouse input initialized.\n");

    // Get initial cursor position
    glfwGetCursorPos(window, &current_mouse_x, &current_mouse_y);
}


double mouse_get_x(void) {
    return current_mouse_x;
}

double mouse_get_y(void) {
    return current_mouse_y;
}

 void mouse_update(window) {
    glfwGetCursorPos(window, &current_mouse_x, &current_mouse_y);     // Future: handle button states, delta movement, etc.
}