#version 330 core

in vec2 position;
in vec2 vTexCoords;

out vec2 fTexCoords;

uniform mat4 projection;

void main()
{
    fTexCoords = vTexCoords;
    gl_Position = projection * vec4(position, 0.0, 1.0);
}
