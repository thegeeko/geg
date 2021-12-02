#include "graphics-context.hpp"

#include <iostream>

namespace Geg {
	GLGraphicsContext::GLGraphicsContext(GLFWwindow* _window):
			windowPtr(_window) {
		GEG_CORE_ASSERT(_window, "Window pointer is null");
	}

	void GLGraphicsContext::init() {
		glfwMakeContextCurrent(windowPtr);
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GEG_CORE_ASSERT(success, "Could not initialize GLAD");
		GEG_CORE_INFO("OpenGl graphics initialized");

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugMsg, NULL);
	}

	void GLGraphicsContext::swapBuffers() {
		glfwSwapBuffers(windowPtr);
	}

	void GLGraphicsContext::glDebugMsg(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* msg,
			const void* data) {
		const char* _source;
		const char* _type;
		const char* _severity;

		switch (source) {
			case GL_DEBUG_SOURCE_API:
				_source = "API";
				break;

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				_source = "WINDOW SYSTEM";
				break;

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				_source = "SHADER COMPILER";
				break;

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				_source = "THIRD PARTY";
				break;

			case GL_DEBUG_SOURCE_APPLICATION:
				_source = "APPLICATION";
				break;

			case GL_DEBUG_SOURCE_OTHER:
				_source = "UNKNOWN";
				break;

			default:
				_source = "UNKNOWN";
				break;
		}

		switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				_type = "ERROR";
				break;

			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				_type = "DEPRECATED BEHAVIOR";
				break;

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				_type = "UDEFINED BEHAVIOR";
				break;

			case GL_DEBUG_TYPE_PORTABILITY:
				_type = "PORTABILITY";
				break;

			case GL_DEBUG_TYPE_PERFORMANCE:
				_type = "PERFORMANCE";
				break;

			case GL_DEBUG_TYPE_OTHER:
				_type = "OTHER";
				break;

			case GL_DEBUG_TYPE_MARKER:
				_type = "MARKER";
				break;

			default:
				_type = "UNKNOWN";
				break;
		}

		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:
				_severity = "HIGH";
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				_severity = "MEDIUM";
				break;

			case GL_DEBUG_SEVERITY_LOW:
				_severity = "LOW";
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				_severity = "NOTIFICATION";
				break;

			default:
				_severity = "UNKNOWN";
				break;
		}

		std::cout
				<< id
				<< ": "
				<< _type
				<< " of "
				<< _severity
				<< " severity, raised from "
				<< _source
				<< ": "
				<< msg
				<< std::endl;
	}
}		 // namespace Geg
