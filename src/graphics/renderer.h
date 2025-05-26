// filepath: src/graphics/renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h> // For GLFWwindow
#include "simulation/entity.h" // For Entity struct
#include <stddef.h>            // For size_t

// Initializes GLFW, GLAD, and creates a window.
// Returns the GLFWwindow pointer or NULL on failure.
GLFWwindow* renderer_init(int width, int height, const char* title);

// Checks if the main loop should continue running.
// Polls events.
int renderer_should_run(GLFWwindow* window);

// Prepares for a new frame (clears screen).
void renderer_begin_frame(void);

// Finalizes the frame (swaps buffers).
void renderer_end_frame(GLFWwindow* window);

// Draws all entities.
// entities: Pointer to the array of entities.
// count: Number of entities to draw.
void renderer_draw_entities(const Entity* entities, size_t count);

// Cleans up renderer resources.
void renderer_cleanup(void);

#endif // RENDERER_H