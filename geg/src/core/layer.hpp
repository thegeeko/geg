#pragma once

namespace Geg {
	class Layer {
	public:
		Layer(const std::string &name = "Layer"):
				debugName(name){};
		virtual ~Layer() = default;

		virtual void onAttach(){};
		virtual void onDetach(){};
		virtual void onUpdate(float deltaTime){};
		virtual void onUiUpdate(float delatTime){};
		virtual void onEvent(Event &event) {}
		virtual const std::string &getName() const { return debugName; }

	protected:
		std::string debugName;
	};

}		 // namespace Geg
