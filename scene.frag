#version 330 core

in vec2 uv;
in vec3 tangentFragPos;

#define NR_POINT_LIGHTS 2  
in struct PointLight {    
    vec3 position;

    vec3 color;
} tangentLights[NR_POINT_LIGHTS];

uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

uniform sampler2D d_tex;
uniform sampler2D b_tex;
uniform sampler2D s_tex;

// constants
vec3 viewPosition = vec3(0.0, 0.0, 0.0); //dome centre

// attenuation constants
float const_Att = 1.0;
float lin_Att = 0.01;
float quadr_Att = 0.0002;

void main()
{
	// convert the rgb value from bumpmap to a vec3(x,y,z) in tangent space
	vec3 norm = texture(b_tex,uv.st).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	vec4 texColor = texture(d_tex, uv.st); //rgba = vec4(.05,0.5,0.5,rgba.a);
	vec3 fragColor = ambient * texColor.rgb;

	for(int i = 0; i < NR_POINT_LIGHTS; i++){

		// light dir for diffuse, spectral. lightdist for attenuation
		vec3 lightDir = normalize(tangentLights[i].position - tangentFragPos);
		float lightDist = length(tangentLights[i].position - tangentFragPos);

		float attenuation = 1.0/(const_Att + lin_Att * lightDist + quadr_Att*lightDist*lightDist);

		float diffuse = max(dot(norm, lightDir), 0.0);

		fragColor += attenuation * tangentLights[i].color * (texColor.rgb *  diffuse);

	}
	gl_FragColor = vec4(fragColor, texColor.a);
}