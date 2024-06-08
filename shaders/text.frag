#version 460 core

in vec2 v_tex_coords;
in vec4 v_color;
flat in uint v_texture_unit;

out vec4 color;

uniform sampler2D u_sampler[32];


void main()
{
    vec4 glyph_sample = vec4(1, 1, 1, texture(u_sampler[v_texture_unit], v_tex_coords).r);
    color = v_color * glyph_sample;
}