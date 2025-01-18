#version 330 core
out vec4 FragColor;

in vec3 ourColor; // Recibir el color del vertex shader

void main()
{
    FragColor = vec4(ourColor, 1.0); // Usar el color recibido, con alfa = 1.0
}
