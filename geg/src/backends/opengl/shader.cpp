#include "shader.hpp"
#include "glad/glad.h"

namespace Geg {
GLShader::GLShader(std::string vertPath, std::string fragPath) {

	// init
	r_Id = glCreateProgram();
	fragShaderData = GLShaderData();
	vertShaderData = GLShaderData();

	// setting shaders up
	vertShaderData.src = Utils::readFileAsString(vertPath);
	vertShaderData.type = GL_VERTEX_SHADER;
	vertShaderData.id = glCreateShader(vertShaderData.type);

	fragShaderData.src = Utils::readFileAsString(fragPath);
	fragShaderData.type = GL_FRAGMENT_SHADER;
	fragShaderData.id = glCreateShader(fragShaderData.type);

	// compile and linking
	compile(vertShaderData);
	compile(fragShaderData);

	glLinkProgram(r_Id);
	glValidateProgram(r_Id);
	glDeleteShader(vertShaderData.id);
	glDeleteShader(fragShaderData.id);
}

GLShader::~GLShader() {
	glDeleteProgram(r_Id);
}

void GLShader::compile(GLShaderData shader) {
	const char *src = shader.src.c_str();
	glShaderSource(shader.id, 1, &src, nullptr);
	glCompileShader(shader.id);
	glAttachShader(r_Id, shader.id);

	int result;
	glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

	GEG_CORE_ASSERT(result, "can't compile the shaders");
}

void GLShader::bind() const {
	glUseProgram(r_Id);
}

void GLShader::unBind() const {
	glUseProgram(0);
}

} // namespace Geg
