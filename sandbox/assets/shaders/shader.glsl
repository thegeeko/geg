###vertex

#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 fragColor;

layout(push_constant) uniform Push {
	mat4 model;
} push;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	mat4 proj;
	mat4 view;
	mat4 projView;
} globalUbo;


void main() {
  gl_Position = globalUbo.proj * globalUbo.view * push.model * vec4(inPosition, 1.0);
  fragColor = vec4(0.8, 0.3, .8, 1.0);
}

###fragment

#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
  outColor = vec4(fragColor);
}
