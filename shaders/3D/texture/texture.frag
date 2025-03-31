#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the fragment position of the light
in vec4 fragPosLight;

// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;
uniform float farPlane;

vec4 pointLight() {	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 0.7;
	float b = 0.3;
	float inten = 1.0 / (a * dist * dist + b * dist + 1.0);

	// ambient lighting
	float ambient = 0.10;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// specular lighting
	float specular = 0.0;
	if(diffuse != 0.0) {
		float specularLight = 0.50;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0), 16);
		specular = specAmount * specularLight;
	};

	// Shadow value
	float shadow = 0.0;
	vec3 fragToLight = crntPos - lightPos;
	float currentDepth = length(fragToLight);
	float bias = max(0.5 * (1.0 - dot(normal, lightDirection)), 0.05); 

	// Not really a radius, more like half the width of a square
	int sampleRadius = 2;
	float offset = 0.02;
	for(int z = -sampleRadius; z <= sampleRadius; z++) {
		for(int y = -sampleRadius; y <= sampleRadius; y++) {
			for(int x = -sampleRadius; x <= sampleRadius; x++) {
				float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
				// Remember that we divided by the farPlane?
				// Also notice how the currentDepth is not in the range [0, 1]
				closestDepth *= farPlane;
				if(currentDepth > closestDepth + bias)
					shadow += 1.0;
			}
		}
	}
	// Average shadow
	shadow /= pow((sampleRadius * 2 + 1), 3);

	return (texture(diffuse0, texCoord) * (diffuse * (1.0 - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0 - shadow) * inten) * lightColor;
}

vec4 directLight() {
	// ambient lighting
	float ambient = 0.20;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// specular lighting
	float specular = 0.0;
	if(diffuse != 0.0) {
		float specularLight = 0.50;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0), 16);
		specular = specAmount * specularLight;
	};

	// Shadow value
	float shadow = 0.0;
	// Sets lightCoords to cull space
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0) {
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.025 * (1.0 - dot(normal, lightDirection)), 0.0005);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++) {
			for(int x = -sampleRadius; x <= sampleRadius; x++) {
				float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if(currentDepth > closestDepth + bias)
					shadow += 1.0;
			}
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}

	return (texture(diffuse0, texCoord) * (diffuse * (1.0 - shadow) + ambient) + texture(specular0, texCoord).r * specular * (1.0 - shadow)) * lightColor;
}

vec4 spotLight() {
	// controls how big the area that is lit up is
	float outerCone = 0.70;
	float innerCone = 0.75;

	// ambient lighting
	float ambient = 0.10;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// specular lighting
	float specular = 0.0;
	if(diffuse != 0.0) {
		float specularLight = 0.50;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0), 16);
		specular = specAmount * specularLight;
	};

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0, -1.0, 0.0), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

	// Shadow value
	float shadow = 0.0;
	// Sets lightCoords to cull space
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0) {
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.0025 * (1.0 - dot(normal, lightDirection)), 0.00005);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++) {
			for(int x = -sampleRadius; x <= sampleRadius; x++) {
				float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if(currentDepth > closestDepth + bias)
					shadow += 1.0;
			}
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}

	return (texture(diffuse0, texCoord) * (diffuse * (1.0 - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0 - shadow) * inten) * lightColor;
}

void main() {
	// outputs final color
	FragColor = pointLight();
}