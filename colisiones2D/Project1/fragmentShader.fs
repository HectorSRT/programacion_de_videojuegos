#version 330 core

out vec4 FragColor; // Color final del fragmento

in vec3 ourColor; // Color recibido del vertex shader

void main()
{
    FragColor = vec4(ourColor, 1.0); // Usa el color interpolado con alpha = 1.0
}
