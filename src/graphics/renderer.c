// filepath: src/graphics/renderer.c
#include <glad/glad.h> // Must be included before GLFW
#include "renderer.h"
#include <stdio.h>     // For fprintf, printf

GLFWwindow* renderer_init(int width, int height, const char* title) {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: Failed to initialize GLFW\n");
        return NULL;
    }

    // Request OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "ERROR: Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }

    glViewport(0, 0, width, height);
    printf("Renderer initialized. OpenGL Vendor: %s, Renderer: %s, Version: %s\n",
           glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
    
    return window;
}

int renderer_should_run(GLFWwindow* window) {
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
}

void renderer_begin_frame(void) {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark background
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_end_frame(GLFWwindow* window) {
    glfwSwapBuffers(window);
}

void renderer_cleanup(void) {
    glfwTerminate();
    printf("Renderer cleaned up.\n");
}