#version 330 core

in vec3 vPosition;
in vec4 vColor;
in vec2 vTexCoords;

out vec4 fColor;
out vec2 fTexCoords;

uniform mat4 projection;

void main()
{
    fTexCoords = vTexCoords;
    fColor = vColor;
    gl_Position = projection * vec4(vPosition, 1.0);
}
