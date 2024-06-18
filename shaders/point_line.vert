#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;
layout (std140, binding = 0) uniform ViewProjectionMatrix
{
    mat4 u_view_proj;
};

out vec4 v_color;

void main()
{
    gl_Position = u_view_proj * vec4(position, 0, 1);
    v_color = color;
}
