#version 450
layout (location = 0) in vec3 aPos;

uniform mat4 cam;

out vec4 vertexColor;

void main() {
	gl_Position = cam * vec4(aPos, 1.0);
}
