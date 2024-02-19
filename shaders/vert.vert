#version 460

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

out vec4 v_color;

void main()
{
    gl_Position = vec4(position, 0, 1);
    v_color = vec4(color, 1);
}