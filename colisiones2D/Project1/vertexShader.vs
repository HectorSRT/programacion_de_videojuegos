#version 330 core

layout(location = 0) in vec3 aPos;  // Posici�n de los v�rtices
layout(location = 1) in vec3 aColor; // Color de los v�rtices

out vec3 ourColor; // Variable para pasar el color al fragment shader

uniform mat4 transform; // Matriz de transformaci�n

void main()
{
    gl_Position = transform * vec4(aPos, 1.0); // Aplica la transformaci�n a la posici�n
    ourColor = aColor; // Asigna el color a la variable de salida
}
