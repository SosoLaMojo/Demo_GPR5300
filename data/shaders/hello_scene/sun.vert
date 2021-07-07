#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTex;

out vec2 out_tex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    out_tex = aTex;

    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}