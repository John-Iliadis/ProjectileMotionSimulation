#version 460 core

in vec2 v_tex_coords;

out vec4 color;

uniform sampler2D text;
uniform vec3 u_text_color;


void main()
{
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(text, v_tex_coords).r);
    color = vec4(u_text_color, 1.f) * sampled;
}