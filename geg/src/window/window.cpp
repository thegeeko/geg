#include "window.hpp"

#include "GLFW/glfw3.h"
#include "core/core.hpp"

namespace Geg {
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char *description) {
		GEG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(WindowProps &props) {
		if (s_GLFWWindowCount == 0) {
			bool success = glfwInit();
			GEG_CORE_ASSERT(success, "Could not initialize GLFW!");

			if (props.windowedFullscreen || props.fullscreen) {
				const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				info.name = props.name;
				info.height = mode->height;
        info.width = mode->width;
			} else {
				info.name = props.name;
				info.height = props.height;
				info.width = props.width;
			}

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			windowPtr = glfwCreateWindow(
					info.width,
					info.height,
					props.name.c_str(),
					nullptr,
					nullptr);
			++s_GLFWWindowCount;

      GEG_ASSERT(windowPtr, "Failed to create window");

			context = GraphicsContext::create(windowPtr);
			context->init();

			glfwSetWindowUserPointer(windowPtr, &info);
			setVsync(true);

			GEG_CORE_INFO("window {} created", props.name);
			glfwSetWindowCloseCallback(windowPtr, [](GLFWwindow *w) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(w);
				WindowCloseEvent e{};
				wi.eventCallback(e);
			});

			glfwSetCursorPosCallback(windowPtr, [](GLFWwindow *w, double x, double y) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(w);
				MouseMovedEvent e(x, y);
				wi.eventCallback(e);
			});

			glfwSetWindowSizeCallback(windowPtr, [](GLFWwindow *window, int width, int height) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(window);
				wi.width = width;
				wi.height = height;

				WindowResizeEvent e(width, height);
				wi.eventCallback(e);
			});

			glfwSetMouseButtonCallback(windowPtr, [](GLFWwindow *window, int button, int action, int mods) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(window);

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
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float)xOffset, (float)yOffset);
				wi.eventCallback(e);
			});

			glfwSetCursorPosCallback(windowPtr, [](GLFWwindow *window, double xPos, double yPos) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(window);

				MouseMovedEvent e((float)xPos, (float)yPos);
				wi.eventCallback(e);
			});

			glfwSetKeyCallback(windowPtr, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(window);

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

			glfwSetCharCallback(windowPtr, [](GLFWwindow *window, uint32_t keycode) {
				WindowInfo &wi = *(WindowInfo *)glfwGetWindowUserPointer(window);
				KeyTappedEvent e(keycode);
				wi.eventCallback(e);
			});
		}
	}

	Window::~Window() {
		delete context;

		glfwDestroyWindow(windowPtr);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}

		GEG_CORE_INFO("window destroyed");
	}

	void Window::onUpdate() {
		glfwPollEvents();
		context->swapBuffers();
	}

	void Window::setVsync(bool state) {
		/* state ? glfwSwapInterval(1) : glfwSwapInterval(0); */
		info.VSync = state;
	}
}		 // namespace Geg
