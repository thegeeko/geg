#pragma once

#include "event.hpp"

#define IMPL_EVENT_TYPE(ev) const static EventType staticEventType = ev;\
                            EventType eventType = ev;\
                            [[nodiscard]] EventType getEventType() const override {return eventType;}

namespace geg {

	// ---------------------- window & app events ---------------------------------------------------- //
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		IMPL_EVENT_TYPE(EventType::WindowClose)

		std::string name = "Window close event";
		char categoryFlags = EventCategoryApplication;

		std::string toString() const override { return name; }

	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(float _w, float _h) : w(_w), h(_h) {}

		IMPL_EVENT_TYPE(EventType::WindowResize)

		std::string name = "Window resized";
		char categoryFlags = EventCategoryApplication;

		inline float getW() const { return w; }

		inline float getH() const { return h; }

		std::string toString() const override {
			std::string str;
			str = "Resized the window to : ";
			str += "w = " + std::to_string(w);
			str += ", h = " + std::to_string(h);
			return str;
		}

	private:
		const float w, h;
	};

	class AppTickEvent : public Event {
	public:
		IMPL_EVENT_TYPE(EventType::AppTick)
		std::string name = "app tick";
		char categoryFlags = EventCategoryApplication;
		std::string toString() const override { return name; }
	};

	class AppUpdateEvent : public Event {
	public:
		IMPL_EVENT_TYPE(EventType::AppUpdate)
		std::string name = "app update";
		char categoryFlags = EventCategoryApplication;
		std::string toString() const override { return name; }
	};

	class AppRenderEvent : public Event {
	public:
		IMPL_EVENT_TYPE(EventType::AppRender)
		std::string name = "app rendered";
		char categoryFlags = EventCategoryApplication;
		std::string toString() const override { return name; }
	};


	// ---------------------- window & app events ---------------------------------------------------- //




	// ---------------------- keyboard events ---------------------------------------------------- //


	class KeyPressedEvent : public Event {
	public:
		IMPL_EVENT_TYPE(EventType::KeyPressed);

		KeyPressedEvent(int _keycode, int _repeatCount) : keyCode(_keycode), repeatCount(_repeatCount) {}
		inline int GetRepeatCount() const { return repeatCount; }
		char categoryFlags = EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard;
		int getKeyCode() {return keyCode;}

		std::string toString() const override {
			std::string str = "pressed key " + std::to_string(keyCode);
			str += " " + std::to_string(repeatCount) + "times";
			return str;
		}

	private:
		int repeatCount;
		int keyCode;
	};


	class KeyReleasedEvent : public Event {
	public:
		IMPL_EVENT_TYPE(EventType::KeyReleased);

		KeyReleasedEvent(int _keycode) : keyCode(_keycode) {}
		char categoryFlags = EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard;
		int getKeyCode() {return keyCode;}

		std::string toString() const override {
			std::string str = "released key " + std::to_string(keyCode);
			return str;
		}

	private:
		int keyCode;
	};

	class KeyTappedEvent : public Event {
	public:
		KeyTappedEvent(int _keycode) : keyCode(_keycode) {}

		IMPL_EVENT_TYPE(EventType::KeyTapped);
		char categoryFlags = EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard;
		int getKeyCode() {return keyCode;}

		std::string toString() const override {
			std::string str = "Tapped key " + std::to_string(keyCode);
			return str;
		}

	private:
		int keyCode;
	};

	// ---------------------- keyboard events ---------------------------------------------------- //




	// ---------------------- mouse events ---------------------------------------------------- //

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float _x, float _y) : x(_x), y(_y) {}

		IMPL_EVENT_TYPE(EventType::MouseMoved);
		std::string name = "Mouse Moved event";
		char categoryFlags = EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse;
		inline float getX() const { return x; }
		inline float getY() const { return y; }

		std::string toString() const override {
			std::string str;
			str = "Mouse Moved : ";
			str += "x = " + std::to_string(x);
			str += ", y = " + std::to_string(y);
			return str;
		}

	private:
		const float x, y;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float _xOffset, float _yOffset) : xOffset(_xOffset), yOffset(_yOffset) {}

		IMPL_EVENT_TYPE(EventType::MouseScrolled);
		inline float getXOffset() const { return xOffset; }
		inline float getYOffset() const { return yOffset; }
		char categoryFlags = EventCategoryMouse | EventCategoryInput;

		std::string toString() const override {
			std::string str;
			str = "Mouse Scrolled : ";
			str += "x = " + std::to_string(xOffset);
			str += ", y = " + std::to_string(yOffset);
			return str;
		}
	private:
		float xOffset, yOffset;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int _button) : button(_button) {}

		IMPL_EVENT_TYPE(EventType::MouseButtonPressed);
		inline int getMouseButton() const { return button; }
		char categoryFlags = EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton;

		std::string toString() const override {
			std::string str = "pressed : " + std::to_string(button);
			return str;
		}

	protected:
		int button;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(int _button) : button(_button) {}

		IMPL_EVENT_TYPE(EventType::MouseButtonReleased);
		inline int getMouseButton() const { return button; }
		char categoryFlags = EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton;

		std::string toString() const override {
			std::string str = "released : " + std::to_string(button);
			return str;
		}

	protected:
		int button;
	};

	// ---------------------- mouse events ---------------------------------------------------- //



}
