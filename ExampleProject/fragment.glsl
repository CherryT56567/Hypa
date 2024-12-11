#version 330 core

in vec3 Colours;

out vec4 FragColor;

void main() {
   FragColor = vec4(Colours, 1.0f);
}