#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec3 out_normal;
out vec2 out_tex;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform mat4 inv_model;

void main()
{
    out_tex = aTex;

    mat3 normalMat = mat3(inv_model);

    vec3 T = normalize(normalMat * aTangent);
    vec3 N = normalize(normalMat * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;

    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}

//#version 330 core
//
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTex;
//
//out vec3 out_position;
//out vec3 out_normal;
//out vec2 out_tex;
//out vec3 out_camera_view;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform mat4 inv_model;
//uniform vec3 camera_position;
//
//void main()
//{
//    mat4 pvm = projection * view * model;
//    out_position = (view * model * vec4(aPos, 1.0)).xyz;
//    gl_Position = pvm * vec4(aPos, 1.0);
//    out_tex = aTex;
//    out_normal = vec3(inv_model * vec4(aNormal, 1.0));
//    out_camera_view = camera_position;
//}