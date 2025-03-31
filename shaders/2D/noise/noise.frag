#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// uniform sampler2D tex1;
uniform sampler2D framebuffer0;
// Gets the Texture Unit from the main function
uniform sampler2D tex0;

uniform float x;
vec2 f(float t)
{
	int k = 1;
	// return 0.2 * vec2((1 - cos(t)) * cos(t), ((1 - cos(t)) * sin(t)));
	return 0.1 * vec2((k + 1) * cos(t) - cos((k + 1) * t), (k + 1) * sin(t) - sin((k + 1) * t));
}

float random(vec2 uv)
{
	return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
	// vec4 color = texture(tex0, texCoord);
	// float gray = (color[0] + color[1] + color[2]) / 3;
	// if(random(texCoord + vec2(0.001 + 0.1 * (x + 0.1), 0)) <= 0.8 * gray)
	// {
	// 	FragColor = vec4(vec3(random(texCoord)), 1);
	// }
	// else
	// {
	// 	FragColor = vec4(vec3(random(texCoord + vec2(x, 0) + vec2(texture(framebuffer0, texCoord)[0], 0))), 1);
	// }

	// FragColor = vec4(vec3(random(texCoord + vec2(texture(framebuffer0, texCoord)[0], 0))), 1);
	FragColor = texture(tex0, texCoord);
	// FragColor = texture(framebuffer0, texCoord);

}