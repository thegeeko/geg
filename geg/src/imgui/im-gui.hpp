#ifndef GEG_IM_GUI_HPP
#define GEG_IM_GUI_HPP

#include "imgui.h"
#include "backends/opengl/imgui-opengl.hpp"
#include "core/app.hpp"

namespace Geg {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer(){};
		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void onEvent(Event &event) override;
	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTappedEvent(KeyTappedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);

		float i_Time;
		std::string debugName;
	};
}

#endif //GEG_IM_GUI_HPP
