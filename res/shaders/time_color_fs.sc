$input v_color0

uniform vec4 u_time;

#include "common/common.sh"

void main()
{
	gl_FragColor = v_color0 * (cos(5*u_time[0]) + 1) * 0.5;
}
