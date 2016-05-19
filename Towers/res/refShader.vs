#version 330 core

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;


out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    Normal = mat3(transpose(inverse(model))) * normal;
    Position = vec3(model * vec4(position, 1.0f));
}  