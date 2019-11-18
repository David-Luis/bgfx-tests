$input v_color0, v_texcoord0// in...

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "common/bgfx_shader.sh"
#include "common/shaderlib.sh"

SAMPLER2D(s_texColor, 0);

void main()
{
	vec4 color = texture2D(s_texColor, v_texcoord0);
	
	gl_FragColor.xyz = color.xyz;
	gl_FragColor.w = 1.0;
}
