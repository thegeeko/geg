#pragma once

#include "renderer/shader.hpp"

namespace Geg {

	struct GLShaderData {
	  unsigned int id;
	  unsigned int type;
	  std::string src;
	};


	class GLShader : public Shader {
	public:
		GLShader(std::string vertPath, std::string fragPath);
		~GLShader();

		void bind() const override;
		void unBind() const override;
		void uploadUniform4f(const std::string& name, float f1, float f2, float f3, float f4) const;
		void uploadUniformMat4f(const char* name, const float* mat4) const;
		void uploadUniformVec3f(const std::string name, const float* vec3) const;
	private:
		unsigned int r_Id;
		GLShaderData vertShaderData;
		GLShaderData fragShaderData;
		void compile(GLShaderData shader);
		const int getUniformLocation(const std::string& name) const;
	};

}
