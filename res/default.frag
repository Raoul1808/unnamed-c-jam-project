#version 330 core

in vec4 fColor;
in vec2 fTexCoords;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, fTexCoords) * fColor;
}
