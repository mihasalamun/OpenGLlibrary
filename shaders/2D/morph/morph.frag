#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform float x;
vec2 f(float t)
{
	int k = 1;
	// return 0.2 * vec2((1 - cos(t)) * cos(t), ((1 - cos(t)) * sin(t)));
	return 0.1 * vec2((k + 1) * cos(t) - cos((k + 1) * t), (k + 1) * sin(t) - sin((k + 1) * t));
}

void main()
{
	float x = (texCoord[0] - 0.5);
	float y = (texCoord[1] - 0.5);
	float r = sqrt(x * x + y * y);
	float t = atan(y, x);
	vec2 z = f(t);
	if(r * r < z.x * z.x + z.y * z.y)
	{
		FragColor = texture(tex0, texCoord);
	}
}