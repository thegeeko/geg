#ifndef GEG_LAYER_HPP
#define GEG_LAYER_HPP

namespace Geg {
	class Layer {
	public:
		Layer(const std::string &name = "Layer") : debugName(name){};
		virtual ~Layer()= default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onUiUpdate() {}
		virtual void onEvent(Event &event) {}
		const std::string &getName() const { return debugName; }

	protected:
		std::string debugName;
	};

}

#endif //GEG_LAYER_HPP
