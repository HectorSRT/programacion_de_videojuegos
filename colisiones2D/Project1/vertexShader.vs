#version 330 core

layout(location = 0) in vec3 aPos;  // Posición de los vértices
layout(location = 1) in vec3 aColor; // Color de los vértices

out vec3 ourColor; // Variable para pasar el color al fragment shader

uniform mat4 transform; // Matriz de transformación

void main()
{
    gl_Position = transform * vec4(aPos, 1.0); // Aplica la transformación a la posición
    ourColor = aColor; // Asigna el color a la variable de salida
}
