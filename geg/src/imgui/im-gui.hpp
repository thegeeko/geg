#pragma once

namespace Geg {
	class ImGuiLayer: public Layer {
	 public:
		ImGuiLayer();
		~ImGuiLayer() override = default;
		void onAttach() override;
		void onDetach() override;
		void onUiUpdate() override;

		void begin();
		void end();
	};
}		 // namespace Geg
