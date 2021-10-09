#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Geg {

	struct WindowProps{
		int width, height;
		std::string name;

		WindowProps(int w = 1366, int h = 720, std::string n = "Geg") : width(w), height(h), name(n) {};
	};

	class Window {
	public:
		Window(WindowProps& props);
		~Window();


		void onUpdate();
		[[nodiscard]]	GLFWwindow* getRawWindow () const {return windowPtr;};
		[[nodiscard]] int getWidth () const {return info.width;};
		[[nodiscard]] int getHeight () const {return info.height;};
		[[nodiscard]] bool getVSync() const {return info.VSync;}
		void setVsync (bool state);
		void setEventCallback (const std::function<void(Event&)>& cb) {info.eventCallback = cb;}

	private:
		GLFWwindow* windowPtr;

		struct WindowInfo {
			int width, height;
			bool VSync = true;
			std::string name;
			std::function<void(Event&)> eventCallback;
		};

		WindowInfo info;

	};
}
