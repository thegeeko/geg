###vertex

#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUv;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
	mat4 model;
	mat4 norm;
} push;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	mat4 proj;
	mat4 view;
	mat4 projView;
	vec4 lightDir;
} globalUbo;

void main() {
	const vec3 LIGHT_DIR = normalize(globalUbo.lightDir.xyz);
	const float AMBIENT = 0.02;
	 
	// converting to world space
  vec4 ws_pos = push.model * vec4(inPosition, 1.0);
	vec3 ws_norm = normalize(mat3(push.norm) * inNormal);

	float lightIntensity = AMBIENT + max(dot(ws_norm, LIGHT_DIR), 0);

  fragColor = lightIntensity * inColor;
	gl_Position = globalUbo.projView * ws_pos;
}

###fragment

#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
  outColor = vec4(fragColor, 1);
}
