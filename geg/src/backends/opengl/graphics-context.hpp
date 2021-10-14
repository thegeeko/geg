#pragma once

#include "glad/glad.h"
#include "renderer/graphics-context.hpp"

namespace Geg {
	class GLGraphicsContext: public GraphicsContext {
	 public:
		GLGraphicsContext(GLFWwindow* _window);
		void init() override;
		void swapBuffers() override;

	 private:
		GLFWwindow* windowPtr;

		static void glDebugMsg(
				GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar* msg,
				const void* data);
	};
}		 // namespace Geg
