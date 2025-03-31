#version 460 core

layout (location = 0) in vec2 aPos;
// Texture Coordinates
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 camMatrix;
out vec4 color;

void main()
{
	color = vec4(aColor, 1.0f);
	gl_Position = camMatrix * model * vec4(aPos, 0.0f, 1.0f);
}