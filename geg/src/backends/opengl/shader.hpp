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
	private:
		unsigned int r_Id;
		GLShaderData vertShaderData;
		GLShaderData fragShaderData;
		void compile(GLShaderData shader);
	};
}
