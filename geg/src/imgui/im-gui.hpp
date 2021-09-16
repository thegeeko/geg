#ifndef GEG_IM_GUI_HPP
#define GEG_IM_GUI_HPP




#include "imgui.h"
#include "core/app.hpp"

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

#endif //GEG_IM_GUI_HPP
