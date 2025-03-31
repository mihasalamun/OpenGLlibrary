#version 460 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec3 aColor;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;
// Outputs the fragment position of the light
out vec4 fragPosLight;

out vec4 color;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;
// Imports the light matrix
uniform mat4 lightProjection;

void main()
{
	// calculates current position
	crntPos = vec3(model * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = mat3(model) * aNormal;

	color = vec4(aColor, 1.0f);

	// Calculates the position of the light fragment for the fragment shader
	fragPosLight = lightProjection * vec4(crntPos, 1.0f);

	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}