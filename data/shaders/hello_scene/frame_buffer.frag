#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 out_tex;

uniform sampler2D screenTexture;

void main()
{
	//FragColor = texture(screenTexture, out_tex);
	FragColor = vec4(vec3(1.0 - texture(screenTexture, out_tex)), 1.0);
}