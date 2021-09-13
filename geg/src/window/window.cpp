
#include "window.hpp"

namespace Geg {
	//@TODO add better event dispatching
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char *description) {
		GEG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(WindowProps props) {
		info.name = props.name;
		info.height = props.height;
		info.width = props.width;

		if (s_GLFWWindowCount == 0) {
			int success = glfwInit();
			GEG_CORE_ASSERT(success, "Could not initialize GLFW!")
			glfwSetErrorCallback(GLFWErrorCallback);
			windowPtr = glfwCreateWindow((int) props.width, (int) props.height, props.name.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
			glfwSetWindowUserPointer(windowPtr, &info);
			setVsync(true);


			GEG_CORE_INFO("window {} created", props.name);
			glfwSetWindowCloseCallback(windowPtr, [](GLFWwindow* window){
				WindowInfo wi = *(WindowInfo *)glfwGetWindowUserPointer(window);
				WindowCloseEvent e{};
				wi.eventCallback(e);
			});
		}

	}

	Window::~Window() {
		glfwDestroyWindow(windowPtr);
		--s_GLFWWindowCount;

		GEG_CORE_WARN("window destroyed");

		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}
	}

	void Window::onUpdate() {
		glfwPollEvents();
		//glClearColor(0.7, 0.3, 0.54, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::setVsync(bool state) {
		if (state)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		info.VSync = state;
	}
}