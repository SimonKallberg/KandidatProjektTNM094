#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;


uniform mat4 MVP;
uniform mat4 model;

#define NR_POINT_LIGHTS 2  
struct PointLight {    
    vec3 position;

    vec3 color;
}; 
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec2 uv;
out vec3 tangentFragPos;
out PointLight tangentLights[NR_POINT_LIGHTS];

void main()
{
	// texture stuff
	uv = tex;

	//  to tangent space for bumpmap
	vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0)));

	mat3 TBN = transpose(mat3(T, B, N));
	
	tangentFragPos = TBN * vec3(model * vec4(pos, 1.0));
	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		tangentLights[i].position = TBN * pointLights[i].position;
		tangentLights[i].color = pointLights[i].color;
	}

	gl_Position =  MVP * model * vec4(pos, 1.0);
}