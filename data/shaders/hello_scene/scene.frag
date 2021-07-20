#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec3 out_position;
in vec2 out_tex;

uniform sampler2D TexDiffuse;
uniform sampler2D TexNormal;

uniform vec3 viewPos;
uniform vec3 lightPos;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 Normal;

void main()
{
	vec3 normal;
	normal = texture(TexNormal, out_tex).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// get Diffuse color
	vec3 color = texture(TexDiffuse, out_tex).rgb;

	// ambiant
	vec3 ambiant = 0.1 * color;

	// diffuse
	vec3 lightDir;
	vec3 viewDir;
	lightDir = normalize(TangentLightPos - TangentFragPos);
	viewDir = normalize(TangentViewPos - TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	// specular
	vec3 refletDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	
	vec3 specular = vec3(0.2) * spec;
	FragColor = vec4(ambiant + diffuse + specular, 1.0);
	BrightColor = vec4(0.0);
}