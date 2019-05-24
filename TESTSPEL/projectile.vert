#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 MVP;
uniform mat4 model;

out vec2 uv;

void main()
{
	uv = tex;

	gl_Position =  MVP * model * vec4(pos, 1.0);
}