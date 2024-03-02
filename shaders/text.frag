#version 460 core

in vec2 v_tex_coords;
in vec4 v_color;
in uint v_texture_unit;

out color;

uniform sampler2D u_sampler[32];


void main()
{
    color = texture(sampler[v_texture_unit], u_tex_coords) * u_color;
}