#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec3 out_position;
in vec3 out_normal;
in vec2 out_tex;
in vec3 out_camera_view;

uniform sampler2D Diffuse;
uniform sampler2D Specular;
uniform float specular_pow;
uniform vec3 specular_vec;

const float ambientStrength = 0.1;
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 lightPos = vec3(0.0, 0.0, 0.0);

void main()
{
     // Compute ambient light.
     vec3 ambient = ambientStrength * lightColor;
     
     // Compute diffuse light.
     vec3 normal = normalize(out_normal);
     vec3 lightDirection = normalize(lightPos - out_position);
     float diff = max(dot(out_normal, lightDirection), 0.0);
     vec3 diffuse = diff * lightColor;
     
     // Compute specular light.
     vec3 view_direction = normalize(out_camera_view - out_position);
     vec3 reflection_direction = reflect(-lightDirection, out_normal);
     float spec = pow(max(dot(view_direction, reflection_direction), 0.0), specular_pow);
     vec3 specular_tex = specular_vec * spec * lightColor;
     vec3 result_diffuse_ambient = (ambient + diffuse) * texture(Diffuse, out_tex).rgb;

     // Total light
     vec3 result_specular = specular_tex * texture(Specular, out_tex).r;
     FragColor = vec4(result_diffuse_ambient + result_specular, 1.0);
     BrightColor = vec4(0.0);
}