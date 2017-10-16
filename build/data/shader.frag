#version 330

in vec2 out_tex_coord;
in vec4 VertPos;

uniform sampler2D textureObject;                                                                

void main(void)
{
	gl_FragColor = texture2D(textureObject, out_tex_coord);
}