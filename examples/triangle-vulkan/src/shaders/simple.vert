#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform  GlobalUbo{
	vec4 cam1;
	vec4 color;
} globalUbo;

void main() {
  gl_Position = vec4(inPosition, 1.0);
  fragColor = globalUbo.color;
}
