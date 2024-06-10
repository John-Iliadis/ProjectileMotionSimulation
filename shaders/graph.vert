#version 460

layout (location = 0) in vec2 position;

uniform mat4 u_view_proj;

void main()
{
    gl_Position = u_view_proj * vec4(position, 0, 1);
}
