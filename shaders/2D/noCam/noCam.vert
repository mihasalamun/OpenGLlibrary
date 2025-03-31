#version 460 core

// Positions/Coordinates
layout(location = 0) in vec2 aPos;
// Texture Coordinates
layout(location = 1) in vec2 aTex;

// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

void main()
{
	gl_Position = vec4(aPos, 0.0, 1.0);
	texCoord = aTex;
}