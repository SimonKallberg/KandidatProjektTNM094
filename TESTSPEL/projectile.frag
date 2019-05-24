#version 330 core

in vec2 uv;

uniform sampler2D d_tex;

void main()
{
	gl_FragColor = texture(d_tex, uv.st);
}