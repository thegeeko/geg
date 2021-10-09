#pragma once

#include "geg-pch.hpp"
#include "imgui.h"
#include "core/layer.hpp"


namespace Geg {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;
		void onAttach() override;
		void onDetach() override;
		void onUiUpdate() override;

		void begin();
		void end();

	private:
		std::string debugName;
	};
}
