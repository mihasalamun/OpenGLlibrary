#version 460 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// // Colors
// layout (location = 2) in vec3 aColor;
// // Texture Coordinates
// layout (location = 3) in vec2 aTex;

uniform mat4 model;
uniform mat4 camMatrix;

out vec3 texCoords;

void main()
{
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);;
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}