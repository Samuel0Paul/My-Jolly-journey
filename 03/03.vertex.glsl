#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 in_texCoord;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(position, 1.0);
    ourColor = color;
    texCoord = in_texCoord;
}
