#version 330 core
out vec4 FragColor;

in vec3 ourColor; // Color interpolado desde el shader de vértices

void main()
{
    FragColor = vec4(ourColor, 1.0); // Color de salida
}
