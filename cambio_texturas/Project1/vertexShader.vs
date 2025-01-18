#version 330 core
layout (location = 0) in vec3 tPos; // Posición del vértice
layout (location = 1) in vec2 aTexture1;

out vec4 ourColor; // Variable de salida para enviar color al fragment shader
out vec2 ourTexture1;

void main()
{
    gl_Position = vec4(tPos.x, tPos.y, tPos.z, 1.0); // Posición en espacio de recorte
    //ourColor = vec4(aColor,1.0f); // Pasar el color al shader de fragmento
    ourTexture1 = aTexture1;
}
