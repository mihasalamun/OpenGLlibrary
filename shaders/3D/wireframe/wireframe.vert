#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 camMatrix;
uniform mat4 model;

uniform float scale;

void main()
{
	vec3 crntPos = vec3(model * vec4(aPos * scale, 1.0f));
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}