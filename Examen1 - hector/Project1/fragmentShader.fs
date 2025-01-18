#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 vertexColor;

uniform sampler2D Texture1;
uniform bool useTexture;

void main()
{
    if (useTexture) {
        FragColor = texture(Texture1, TexCoord);
    } else {
        FragColor = vec4(vertexColor, 1.0f);  // Color amarillo
    }
}



