#version 330 core
layout (location = 0) in vec3 position;
attribute vec3 position;
attribute vec3 normal;

out vec3 Normal;
out vec3 FragPos;
out vec3 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;  
	TexCoords = position;
} 