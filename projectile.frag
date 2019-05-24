#version 330 core

in vec2 uv;

uniform vec4 color;

uniform sampler2D d_tex;

void main()
{
	gl_FragColor = color * texture(d_tex, uv.st);
}