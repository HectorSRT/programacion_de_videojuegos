#version 330 core
layout (location = 0) in vec3 aPos;   // Atributo posici�n
layout (location = 1) in vec3 aColor; // Atributo color

out vec3 ourColor; // Color que ser� enviado al shader de fragmentos

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // Pasamos el color al shader de fragmentos
}
