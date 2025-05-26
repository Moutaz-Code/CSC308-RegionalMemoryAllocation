#include <glad/glad.h>
#include "renderer.h"
#include "config.h"     // For ENTITY_POINT_SIZE
#include <stdio.h>
#include <stdlib.h>     // For malloc, free (for shader source reading)
#include "input/mouse.h" // Already there

// Shader program ID
static GLuint shader_program_points;
// Vertex Array Object (VAO) and Vertex Buffer Object (VBO) for points
static GLuint point_vao;
static GLuint point_vbo;
// Uniform locations
static GLint proj_matrix_location;
static GLint point_size_location;
static GLint point_color_location;

// --- Shader Helper Functions ---
// Reads a shader file into a string. Remember to free the returned string.
static char* read_shader_source(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "ERROR: Could not open shader file: %s\n", filepath);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "ERROR: Could not allocate memory for shader source: %s\n", filepath);
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

// Compiles a shader and checks for errors.
static GLuint compile_shader(GLenum type, const char* source, const char* shader_name) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR: Shader compilation failed (%s):\n%s\n", shader_name, info_log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// Links shaders into a program and checks for errors.
static GLuint create_shader_program(const char* vertex_path, const char* fragment_path) {
    char* vertex_shader_source = read_shader_source(vertex_path);
    char* fragment_shader_source = read_shader_source(fragment_path);
    if (!vertex_shader_source || !fragment_shader_source) {
        free(vertex_shader_source); // free if not null
        free(fragment_shader_source); // free if not null
        return 0;
    }

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source, "VERTEX");
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source, "FRAGMENT");
    
    free(vertex_shader_source);
    free(fragment_shader_source);

    if (vertex_shader == 0 || fragment_shader == 0) {
        glDeleteShader(vertex_shader); // Safe to call glDeleteShader with 0
        glDeleteShader(fragment_shader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "ERROR: Shader program linking failed:\n%s\n", info_log);
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}
// --- End Shader Helper Functions ---

GLFWwindow* renderer_init(int width, int height, const char* title) {
    // Ensure this part is successful before proceeding
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

    mouse_init(window);

    // --- Create shader program for points ---
    shader_program_points = create_shader_program("src/graphics/shaders/point.vert", 
                                                  "src/graphics/shaders/point.frag");
    if (shader_program_points == 0) {
        fprintf(stderr, "Failed to create point shader program.\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }
    // Get uniform locations
    proj_matrix_location = glGetUniformLocation(shader_program_points, "uProjection");
    point_size_location = glGetUniformLocation(shader_program_points, "uPointSize");
    point_color_location = glGetUniformLocation(shader_program_points, "uPointColor");
    printf("Uniform Locations: proj: %d, size: %d, color: %d\n",
           proj_matrix_location, point_size_location, point_color_location);

    // --- Setup VAO and VBO for points ---
    glGenVertexArrays(1, &point_vao);
    glGenBuffers(1, &point_vbo);

    glBindVertexArray(point_vao);
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);

    // Vertex attribute for position (location 0 in shader)
    // Data is vec2 (x, y)
    // Stride is 2 * sizeof(float) because data is tightly packed: x, y, x, y, ...
    // Offset is 0 because the first component (x) is at the beginning.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Enable vertex attribute at location 0

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0);             // Unbind VAO

    glEnable(GL_PROGRAM_POINT_SIZE); // Allows gl_PointSize in vertex shader to work
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
    glClearColor(0.4f, 0.05f, 0.1f, 1.0f); // Dark background
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_end_frame(GLFWwindow* window) {
    glfwSwapBuffers(window);
}

void renderer_draw_entities(const Entity* entities, size_t count) {
    // DEBUG: Print the count received by the renderer
    //printf("Renderer: Drawing %zu entities.\n", count);

    if (!entities || count == 0) {
        return;
    }

    // Prepare data: extract x, y from entities into a flat array
    // This is inefficient for many entities every frame if using malloc/free here.
    // For now, we'll do it directly. Later, with region allocators, this might change.
    float* positions = (float*)malloc(count * 2 * sizeof(float));
    if (!positions) {
        fprintf(stderr, "ERROR: Failed to allocate memory for entity positions.\n");
        return;
    }
    for (size_t i = 0; i < count; ++i) {
        positions[i * 2 + 0] = entities[i].x;
        positions[i * 2 + 1] = entities[i].y;
    }

    // --- THIS IS THE TEST CODE TO ADD ---
    if (count > 0) {
        positions[0] = (float)WINDOW_WIDTH / 2.0f;  // Force X to horizontal center
        positions[1] = (float)WINDOW_HEIGHT / 2.0f; // Force Y to vertical center (according to your Y-down projection)
        printf("Renderer Test: Forcing Entity 0 to (%.2f, %.2f) for GPU.\n", positions[0], positions[1]);
    }
    // --- END OF TEST CODE ---

    glUseProgram(shader_program_points);

    // DEBUG: Print the width/height values being used by the projection matrix
    printf("Renderer: Projection matrix using WINDOW_WIDTH = %.1f, WINDOW_HEIGHT = %.1f\n",
           (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);

    float left = 0.0f;
    float right = (float)WINDOW_WIDTH;
    float bottom = (float)WINDOW_HEIGHT; // Y-coordinate for the "bottom" of the projection volume
    float top = 0.0f;                    // Y-coordinate for the "top" of the projection volume

    float projection_matrix[4][4] = {
        { 2.0f / (right - left), 0.0f,                  0.0f, -(right + left) / (right - left) },
        { 0.0f,                  2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom) },
        { 0.0f,                  0.0f,                 -1.0f,  0.0f                             }, // z-values don't matter much for 2D
        { 0.0f,                  0.0f,                  0.0f,  1.0f                             }
    };
    glUniformMatrix4fv(proj_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]);
    
    // Set point uniforms
    glUniform1f(point_size_location, ENTITY_POINT_SIZE);
    glUniform3f(point_color_location, 0.8f, 0.8f, 0.1f); // Yellowish points

    // --- Bind VAO and VBO, buffer data, and draw ---
    glBindVertexArray(point_vao);
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
    // Buffer the entity position data for this frame
    // GL_DYNAMIC_DRAW is a hint that data will change frequently
    glBufferData(GL_ARRAY_BUFFER, count * 2 * sizeof(float), positions, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, (GLsizei)count);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0);             // Unbind VAO
    glUseProgram(0);                  // Unbind shader program

    free(positions); // Free the temporary buffer
}

void renderer_cleanup(void) {
    glDeleteVertexArrays(1, &point_vao);
    glDeleteBuffers(1, &point_vbo);
    glDeleteProgram(shader_program_points);

    glfwTerminate(); // Call this here as it's the renderer's responsibility
    printf("Renderer cleaned up.\n");
}