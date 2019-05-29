#version 330 core

in vec2 uv;

uniform vec4 color;

uniform sampler2D d_tex;

out vec4 out_fragColor;

void main()
{
    out_fragColor = color * texture(d_tex, uv.st);
}
