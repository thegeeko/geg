#pragma once

#include "GLFW/glfw3.h"

namespace Geg {
	class GraphicsContext {
	public:
		virtual ~GraphicsContext() {};
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
		virtual void windowResized(int width, int height) = 0;
		static GraphicsContext* create(GLFWwindow* _windowPtr);
		
	};
}		 // namespace Geg
