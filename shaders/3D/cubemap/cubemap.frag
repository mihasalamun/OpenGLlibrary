#version 460 core

out vec4 FragColor;

uniform samplerCube cubemap;
// Imports the texture coordinates from the Vertex Shader
in vec3 texCoords;

void main()
{             
    FragColor = texture(cubemap, texCoords);
}