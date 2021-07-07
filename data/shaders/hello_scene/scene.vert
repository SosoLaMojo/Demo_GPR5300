#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTangent;

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
    TangentFragPos = TBN * (model * vec4(aPos, 1.0)).xyz;

    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}