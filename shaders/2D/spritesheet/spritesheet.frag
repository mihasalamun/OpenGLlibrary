#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2DArray tex0;
uniform int layer;


void main()
{
	FragColor = texture(tex0, vec3(texCoord, layer));
}