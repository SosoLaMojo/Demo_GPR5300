#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec2 out_tex;

uniform sampler2D Texture;

float intensityBlur = 0.8;

void main()
{
	// normal colors
	FragColor = texture(Texture, out_tex);

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > intensityBlur)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}