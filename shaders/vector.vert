#version 460 core

layout (location = 0) in vec2 position;
layout (std140, binding = 0) uniform ViewProjectionMatrix
{
    mat4 u_view_proj;
};

uniform mat4 u_model;

void main()
{
    gl_Position = u_view_proj * u_model * vec4(position, 0, 1); // todo: check model
}
