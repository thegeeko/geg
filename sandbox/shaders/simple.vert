#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	mat4 proj;
	mat4 view;
	mat4 projView;
} globalUbo;

void main() {
  gl_Position = globalUbo.view * vec4(inPosition, 1.0);
  fragColor = vec4(0.2, 0.8, .6, 1.0);
}
