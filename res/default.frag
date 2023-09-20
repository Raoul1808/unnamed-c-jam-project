#version 330 core

in vec2 fTexCoords;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, fTexCoords) * vec4(1.0, 0.0, 0.0, 1.0);
}