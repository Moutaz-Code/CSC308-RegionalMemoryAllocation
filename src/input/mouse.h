#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h> // For GLFWwindow

// Initializes mouse input handling and registers callbacks.
// Needs the GLFW window to register callbacks.
void mouse_init(GLFWwindow* window);

// Gets the current X position of the mouse cursor in window coordinates.
double mouse_get_x(void);

// Gets the current Y position of the mouse cursor in window coordinates.
double mouse_get_y(void);

// (Optional for now, but good for future)
// Updates mouse state, e.g., for button presses, delta movement.
// For now, it can be empty if we only need cursor position via glfwGetCursorPos.
// void mouse_update(void);

#endif // MOUSE_H