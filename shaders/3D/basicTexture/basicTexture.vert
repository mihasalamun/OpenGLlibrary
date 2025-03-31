#version 460 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Texture Coordinates
layout (location = 1) in vec2 aTex;

// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
	texCoord = aTex;
}