###vertex

#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUv;

layout(location = 0) out vec4 fragColor;

layout(push_constant) uniform Push {
	mat4 model;
	mat4 norm;
} push;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	mat4 proj;
	mat4 view;
	mat4 projView;
} globalUbo;


void main() {
  gl_Position = globalUbo.projView * push.model * vec4(inPosition, 1.0);
  fragColor = inColor;
}

###fragment

#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
  outColor = vec4(fragColor);
}
