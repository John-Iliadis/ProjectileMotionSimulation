#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 tex_coords;

out vec2 v_tex_coords;

uniform mat4 u_view_projection_matrix;

void main()
{
    gl_Position = u_view_projection_matrix * vec4(position, 0, 1);
    v_tex_coords = tex_coords;
}