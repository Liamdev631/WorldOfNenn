// Terrain
Shader.vert
#version 150 // Specify which version of GLSL we are using.

layout(location = 0) in vec3 in_position;
layout(location = 12) in vec3 in_normal;

out vec3 Normal;

void main() 
{
    gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);
	Normal = in_normal;
}