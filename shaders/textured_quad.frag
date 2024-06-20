#version 460 core

in vec2 v_tex_coords;

out vec4 color;

uniform sampler2D u_sampler;

void main()
{
    color = texture(u_sampler, v_tex_coords);
}
