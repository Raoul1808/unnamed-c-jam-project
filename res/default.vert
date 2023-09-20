#version 330 core

in vec2 position;
in vec2 vTexCoords;

out vec2 fTexCoords;

void main()
{
    fTexCoords = vTexCoords;
    gl_Position = vec4(position, 1.0, 1.0);
}
