#version 410

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec4 world_position;
layout(location = 2) in vec4 world_normal;
layout(location = 3) in vec4 view_position;
layout(location = 4) in vec4 view_normal;

uniform sampler2D u_texture_0;	// Diffuse texture
uniform float transparency;
uniform uint obj_class;

//  Output data
layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec4 out_world_position;
layout(location = 2) out vec4 out_world_normal;
layout(location = 3) out vec4 out_view_position;
layout(location = 4) out vec4 out_view_normal;

// MAIN
void main()
{
	vec4 diffuse = texture(u_texture_0, texture_coord);
	if(diffuse.a < 0.9)
		discard;

	if (obj_class == 1) {
		diffuse.a *= transparency;
	}
	if (obj_class == 2) {
		float width = fwidth(diffuse.r);
		diffuse.a = smoothstep(0.5 - width, 0.5 + width, diffuse.r);
	}

	out_diffuse = diffuse;
	out_world_position = world_position;
	out_world_normal.xyz = normalize(world_normal.xyz);
	out_view_position = view_position;
	out_view_normal.xyz = normalize(view_normal.xyz);
}
