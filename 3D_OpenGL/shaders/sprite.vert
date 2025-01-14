#version 330 core
layout (location = 0) in vec3 positionCoord;
layout (location = 1) in vec2 textureCoord;

// layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = textureCoord;
    gl_Position = projection * view * model * vec4(positionCoord, 1.0);
}