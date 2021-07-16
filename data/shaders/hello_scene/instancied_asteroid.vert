#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in mat4 aInstanceMatrix;

out vec3 out_position;
out vec3 out_normal;
out vec2 out_tex;
out vec3 out_camera_view;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;

void main()
{
    mat4 inv_model = transpose(inverse(aInstanceMatrix));

    mat4 pvm = projection * view * aInstanceMatrix;
    out_position = (aInstanceMatrix * vec4(aPos, 1.0)).xyz;
    gl_Position = pvm * vec4(aPos, 1.0);
    out_tex = aTex;
    out_normal = vec3(inv_model * vec4(aNormal, 1.0));
    out_camera_view = camera_position;
}