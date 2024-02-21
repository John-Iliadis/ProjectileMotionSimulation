#version 460

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

out vec4 v_color;

uniform mat4 u_view_projection_matrix;

void main()
{
    gl_Position = u_view_projection_matrix * vec4(position, 0, 1);
    v_color = vec4(color, 1);
}