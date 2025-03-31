#version 460 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 fragment = texture(screenTexture, texCoords);
    FragColor.rgb = fragment.rgb;
}