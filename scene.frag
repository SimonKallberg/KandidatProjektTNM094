#version 330 core

in vec2 uv;
in vec3 tangentFragPos;

uniform sampler2D d_tex;
uniform sampler2D b_tex;

void main()
{
	// convert the rgb value from bumpmap to a vec3(x,y,z) in tangent space
	vec3 norm = texture(b_tex,uv.st).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	vec4 texColor = texture(d_tex, uv.st); //rgba = vec4(.05,0.5,0.5,rgba.a);

	gl_FragColor = texColor;
}