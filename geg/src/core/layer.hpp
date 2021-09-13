#ifndef GEG_LAYER_HPP
#define GEG_LAYER_HPP

namespace Geg {
	class Layer {
	public:
		Layer(const std::string &name = "Layer") : debugName(name){};
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event &event) {}
		const std::string &GetName() const { return debugName; }

	protected:
		std::string debugName;
	};

}

#endif //GEG_LAYER_HPP
