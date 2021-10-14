#pragma once

namespace Geg {
	class Layer {
	 public:
		Layer(const std::string &name = "Layer"):
				debugName(name){};
		virtual ~Layer() = default;

		virtual void onAttach(){};
		virtual void onDetach(){};
		virtual void onUpdate(){};
		virtual void onUiUpdate(){};
		virtual void onEvent(Event &event) {}
		virtual const std::string &getName() const { return debugName; }

	 protected:
		std::string debugName;
	};

}		 // namespace Geg
