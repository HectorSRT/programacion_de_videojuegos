#version 330 core
in vec2 ourTexture1;
in vec2 ourTexture2;

out vec4 FragColor;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main()
{
    FragColor = texture(Texture1, ourTexture1);
};
