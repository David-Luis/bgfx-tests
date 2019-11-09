$input v_color0, v_texcoord0

#include "common/common.sh"

uniform vec4 u_data;

void main()
{
	float time = u_data[0];
	vec2 resolution = vec2(u_data[1], u_data[2]);
	vec2 pixel = v_texcoord0;
	//vec3 color = 0.5 + 0.5*cos(time+v_texcoord0.xyx+vec3(0,2,4));
	//gl_FragColor = vec4(color, 1.0);
	
	vec3 color = vec3(1.0, 0.4, 0.1);
	color *= length( pixel - vec2(0.5, 0.5)); //read this as distance from pixel to center
	gl_FragColor = vec4( color, 1.0 );
}
