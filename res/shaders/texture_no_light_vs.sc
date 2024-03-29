$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "common/bgfx_shader.sh"
#include "common/shaderlib.sh"

void main()
{
	v_color0 = a_color0;
	v_texcoord0 = a_texcoord0;
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
}