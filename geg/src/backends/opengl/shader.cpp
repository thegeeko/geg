#include "shader.hpp"

#include "core/logger.hpp"
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
		const char* src = shader.src.c_str();
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

	const int GLShader::getUniformLocation(const std::string& name) const {
		int location = glGetUniformLocation(r_Id, name.c_str());
		if (location < 0) GEG_CORE_ERROR("can't find uniform [" + name + "]");
		return location;
	}

	void GLShader::uploadUniform4f(const std::string& name, float f1, float f2, float f3, float f4) const {
		glUniform4f(getUniformLocation(name), f1, f2, f3, f4);
	}
	void GLShader::uploadUniformMat4f(const char* name, const float* mat) const {
		bind();
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, mat);
	}
	void GLShader::uploadUniformVec3f(const std::string name, const float* vec) const {
		glUniform3fv(getUniformLocation(name), 1, vec);
	}

}		 // namespace Geg
