#version 330 core
layout (location = 0) in vec3 aPos;  // posición del vértice
layout (location = 2) in vec2 aTexCoord;  // coordenada de la textura
layout (location = 1) in vec3 aColor;  // El color del vértice

out vec3 vertexColor;  // El color que se pasará al fragment shader
out vec2 TexCoord;  // Coordenadas de textura

uniform bool useTexture; // Bandera para saber si usar textura o color

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    vertexColor = aColor;  // Siempre pasamos el color del vértice
}
