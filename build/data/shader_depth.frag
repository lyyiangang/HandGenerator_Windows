#version 330

in vec2 tex_coord;
in highp float depth_val;

uniform sampler2D textureObject;                                                                

vec4 packFloatToVec4i(const float value)
{
	const vec4 bitSh = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
	const vec4 bitMsk = vec4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
	vec4 res = fract(value * bitSh);
	res -= res.xxyz * bitMsk;
	return res;
}
float unpackFloatFromVec4i(const vec4 value)
{
	const vec4 bitSh = vec4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
	return(dot(value, bitSh));
}

void main(void)
{
	gl_FragColor = packFloatToVec4i(depth_val / 40.0);
}