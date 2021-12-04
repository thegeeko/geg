#pragma once

#include "GLFW/glfw3.h"
#include "renderer/graphics-context.hpp"

namespace Geg {

	struct WindowProps {
		int width, height;
		std::string name;

		WindowProps(int w = 1366, int h = 720, std::string n = "Geg"):
				width(w), height(h), name(n){};
	};

	class Window {
	 public:
		Window(WindowProps& props);
		~Window();

		void onUpdate();
		[[nodiscard]] GLFWwindow* getRawWindow() const { return windowPtr; };
		[[nodiscard]] GraphicsContext* getGraphicsContext() const { return context; };
		[[nodiscard]] const int getWidth() const { return info.width; };
		[[nodiscard]] const int getHeight() const { return info.height; };
		[[nodiscard]] const bool getVSync() const { return info.VSync; }
		void setVsync(bool state);
		void setEventCallback(const std::function<void(Event&)>& cb) { info.eventCallback = cb; }

	 private:
		GLFWwindow* windowPtr;
		GraphicsContext* context;
		struct WindowInfo {
			int width, height;
			bool VSync = true;
			std::string name;
			std::function<void(Event&)> eventCallback;
		};

		WindowInfo info;
	};
}		 // namespace Geg
