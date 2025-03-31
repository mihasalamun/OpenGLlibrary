#version 460 core

out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D framebuffer0;

float random(vec2 uv)
{
	return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
	// FragColor = vec4(vec3(random(texCoord)), 1);
	// FragColor = vec4(vec3(random(texCoord + vec2(texture(framebuffer0, texCoord)[0], 0))), 1);
	// FragColor = texture(framebuffer0, texCoord);
	FragColor = texture(tex0, texCoord) * 2;

}