#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 tex_coords;
layout (std140, binding = 0) uniform ViewProjectionMatrix
{
    mat4 u_view_proj;
};

out vec2 v_tex_coords;

void main()
{
    gl_Position = u_view_proj * vec4(position, 0, 1);
    v_tex_coords = tex_coords;
}
