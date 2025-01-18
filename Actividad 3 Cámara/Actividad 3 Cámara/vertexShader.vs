layout(location = 0) in vec3 aPos; // Posici�n del v�rtice
layout(location = 1) in vec3 aColor; // Color del v�rtice

out vec3 ourColor; // Color que se enviar� al fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; // Pasar el color al fragment shader
}
