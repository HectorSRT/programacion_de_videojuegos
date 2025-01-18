#version 330 core

in vec4 ourColor; // Color desde el vértice
out vec4 FragColor;

uniform vec3 dynamicColor;

void main()
{
    FragColor = vec4(dynamicColor, 1.0);
}
