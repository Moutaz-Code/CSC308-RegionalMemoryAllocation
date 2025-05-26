// filepath: src/graphics/shaders/point.frag
#version 330 core
out vec4 FragColor;

uniform vec3 uPointColor; // Color of the points

void main() {
    FragColor = vec4(uPointColor, 1.0);
}