$input v_color0, v_texcoord0

#include "common/common.sh"

uniform vec4 u_data;

#define DRAW(f,lineWidth,lineColor, resolution) \
{\
    float u=1./resolution.y*lineWidth;\
    \
    float value0=f(pos+vec2(-u,0.));\
    float value1=f(pos+vec2(+u,0.));\
    float value2=f(pos+vec2(0.,-u));\
    float value3=f(pos+vec2(0.,+u));\
    \
    float minValue=min(min(value0,value1),min(value2,value3));\
    float maxValue=max(max(value0,value1),max(value2,value3));\
    if(minValue<0. && maxValue>0.){\
        col=lineColor;\
    }\
}


float f1(vec2 pos){
return 3.*pos.x*pos.x-1.0-pos.y;
    //return 3.*pos.x*pos.x-0.5-pos.y;
}
float f2(vec2 pos){
    return sin(pos.x)-pos.y;
}
float f3(vec2 pos){
    return length(pos)-1;
}

void main()
{
	float time = u_data[0] * 0.5;
	vec2 resolution = vec2(u_data[1], u_data[2]);
	vec2 fragCoord = v_texcoord0 * resolution;
	
	//from -1 to 1
    vec2 pos=( 2.*fragCoord - resolution )/resolution.y;
    
    float lineWidth=3.;
    vec3 col=vec3(1.0, 1.0, 1.0);
    
    DRAW(f1,lineWidth,vec3(1.,0.,0.), resolution);
    DRAW(f2,lineWidth,vec3(0.,1.,0.), resolution);
    DRAW(f3,lineWidth,vec3(0.,0.,1.), resolution);

    // Output to screen
    gl_FragColor = vec4(col,1.0);
	
	//gl_FragColor = vec4( color, color, color, 1.0 );
}
