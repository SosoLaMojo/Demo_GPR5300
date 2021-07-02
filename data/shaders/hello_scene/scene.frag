#version 330 core

out vec4 FragColor;

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
}

//#version 330 core
//
//layout(location = 0) out vec4 FragColor;
//
//in vec3 out_position;
//in vec3 out_normal;
//in vec2 out_tex;
//in vec3 out_camera_view;
//
//uniform sampler2D Diffuse;
//uniform sampler2D Specular;
//uniform float specular_pow;
//uniform vec3 specular_vec;
//
//const float ambientStrength = 0.1;
//const vec3 lightColor = vec3(1.0, 1.0, 1.0);
//const vec3 lightPos = vec3(0.0, 0.0, 3.0);
//
//void main()
//{
//     // Compute ambient light.
//     vec3 ambient = ambientStrength * lightColor;
//     
//     // Compute diffuse light.
//     vec3 normal = normalize(out_normal);
//     vec3 lightDirection = normalize(lightPos - out_position);
//     float diff = max(dot(out_normal, lightDirection), 0.0);
//     vec3 diffuse = diff * lightColor;
//     
//     // Compute specular light.
//     vec3 view_direction = normalize(out_camera_view - out_position);
//     vec3 reflection_direction = reflect(-lightDirection, out_normal);
//     float spec = pow(max(dot(view_direction, reflection_direction), 0.0), specular_pow);
//     vec3 specular_tex = specular_vec * spec * lightColor;
//     vec3 result_diffuse_ambient = (ambient + diffuse) * texture(Diffuse, out_tex).rgb;
//
//     // Total light
//     vec3 result_specular = specular_tex * texture(Specular, out_tex).r;
//     FragColor = vec4(result_diffuse_ambient + result_specular, 1.0);
//     FragColor = vec4(texture(Diffuse, out_tex).rgb, 1.0);
//}