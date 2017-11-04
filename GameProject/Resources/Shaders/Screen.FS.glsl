#version 410

layout(location = 0) in vec2 tex_coord;

uniform sampler2D u_texture_0;
uniform ivec2 resolution;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture(u_texture_0, tex_coord);
}