#version 330 core
layout (location = 0) in vec3 positionCoord;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoord;
layout (location = 3) in vec3 normal;

// layout (location = 0) in vec4 vertex;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;
out vec3 vs_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vs_position = vec4(model * vec4(positionCoord, 1.f)).xyz;
    vs_texcoord = textureCoord;
    vs_normal = mat3(model) * normal;
    vs_color = color;
    
    gl_Position = projection * view * model * vec4(positionCoord, 1.0);
}