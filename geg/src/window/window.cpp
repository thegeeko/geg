#include "window.hpp"

namespace Geg {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char *description) {
		GEG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(WindowProps &props) {
		info.name = props.name;
		info.height = props.height;
		info.width = props.width;

		if (s_GLFWWindowCount == 0) {
			bool success = glfwInit();
			GEG_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
			windowPtr = glfwCreateWindow(props.width, props.height, props.name.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
			glfwMakeContextCurrent(windowPtr);
			glfwSetWindowUserPointer(windowPtr, &info);

			success = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			GEG_CORE_ASSERT(success, "Could not initialize GLAD");
			setVsync(true);


			GEG_CORE_INFO("window {} created", props.name);
			glfwSetWindowCloseCallback(windowPtr, [](GLFWwindow *w) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(w);
				WindowCloseEvent e{};
				wi.eventCallback(e);
			});

			glfwSetCursorPosCallback(windowPtr, [](GLFWwindow *w, double x, double y) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(w);
				MouseMovedEvent e(x, y);
				wi.eventCallback(e);
			});

			glfwSetWindowSizeCallback(windowPtr, [](GLFWwindow *window, int width, int height) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(window);
				wi.width = width;
				wi.height = height;

				WindowResizeEvent e(width, height);
				wi.eventCallback(e);
			});

			glfwSetMouseButtonCallback(windowPtr, [](GLFWwindow *window, int button, int action, int mods) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(window);

				switch (action) {
					case GLFW_PRESS: {
						MouseButtonPressedEvent e(button);
						wi.eventCallback(e);
						break;
					}
					case GLFW_RELEASE: {
						MouseButtonReleasedEvent e(button);
						wi.eventCallback(e);
						break;
					}
				}
			});

			glfwSetScrollCallback(windowPtr, [](GLFWwindow *window, double xOffset, double yOffset) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float) xOffset, (float) yOffset);
				wi.eventCallback(e);
			});

			glfwSetCursorPosCallback(windowPtr, [](GLFWwindow *window, double xPos, double yPos) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(window);

				MouseMovedEvent e((float) xPos, (float) yPos);
				wi.eventCallback(e);
			});

			glfwSetKeyCallback(windowPtr, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(window);

				switch (action) {
					case GLFW_PRESS: {
						KeyPressedEvent e(key, 0);
						wi.eventCallback(e);
						break;
					}
					case GLFW_RELEASE: {
						KeyReleasedEvent e(key);
						wi.eventCallback(e);
						break;
					}
					case GLFW_REPEAT: {
						KeyPressedEvent e(key, 1);
						wi.eventCallback(e);
						break;
					}
				}
			});

			glfwSetCharCallback(windowPtr, [](GLFWwindow *window, unsigned int keycode) {
				WindowInfo &wi = *(WindowInfo *) glfwGetWindowUserPointer(window);

				KeyTappedEvent e(keycode);
				wi.eventCallback(e);
			});
		}

	}

	Window::~Window() {
		glfwDestroyWindow(windowPtr);
		--s_GLFWWindowCount;

		GEG_CORE_INFO("window destroyed");

		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}
	}

	void Window::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(windowPtr);
	}

	void Window::setVsync(bool state) {
		state ? glfwSwapInterval(1) : glfwSwapInterval(0);
		info.VSync = state;
	}
}
