// filepath: src/graphics/shaders/point.vert
#version 330 core
layout (location = 0) in vec2 aPos; // Vertex position (x, y)

uniform mat4 uProjection; // Projection matrix to map coordinates to screen space
uniform float uPointSize; // Size of the points to draw

void main() {
    gl_Position = uProjection * vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_PointSize = uPointSize;
}