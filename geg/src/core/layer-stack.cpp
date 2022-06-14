#include "layer-stack.hpp"

namespace geg {
	LayerStack::~LayerStack() {
		for (Layer *layer : layers) {
			layer->onDetach();
			delete layer;
		}
		GEG_CORE_INFO("Layer stack cleared");
	}

	void LayerStack::pushLayer(Layer *layer) {
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layerInsertIndex++;
	}

	void LayerStack::pushOverlay(Layer *overlay) {
		layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer *layer) {
		auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
		if (it != layers.begin() + layerInsertIndex) {
			layer->onDetach();
			layers.erase(it);
			layerInsertIndex--;
		}
	}

	void LayerStack::popOverlay(Layer *overlay) {
		auto it = std::find(layers.begin() + layerInsertIndex, layers.end(), overlay);
		if (it != layers.end()) {
			overlay->onDetach();
			layers.erase(it);
		}
	}
}