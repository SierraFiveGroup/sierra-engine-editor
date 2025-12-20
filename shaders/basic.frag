#version 450 core
in vec3 vColor;

out vec4 FragColor;

void main() {
    // Use vertex color if provided, otherwise default to light gray
    FragColor = vec4(vColor, 1.0);
}