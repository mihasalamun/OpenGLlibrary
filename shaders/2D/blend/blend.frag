#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform float x;

void main()
{
	float a = 1.0 / 8;
	float pixel = 1.0 / 512;

	mat3 kernel = mat3(vec3(-a,-a,-a), 
					   vec3(-a,1,-a),
					   vec3(-a,-a,-a));

	// mat3 kernel = mat3(vec3(a,0,-a), 
	// 				   vec3(a,0,-a),
	// 				   vec3(a,0,-a));
	// FragColor = texture(tex0, texCoord + vec2(x, x));
	FragColor = vec4(0, 0, 0, 1);
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			FragColor += kernel[j][i] * texture(tex0, texCoord + vec2((i - 1) * pixel + x, (j - 1) * pixel)+ vec2(x, x));
		}
	}
	for(int i = 0; i < 4; i++)
	{
		FragColor[i] = abs(FragColor[i]) * 8;
	}
	if(FragColor[0] < 1e-5 && FragColor[1] < 1e-5 && FragColor[2] < 1e-5 )
	{
		FragColor[3] = 0;
	}
}