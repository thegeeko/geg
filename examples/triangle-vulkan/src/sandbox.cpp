#include "sandbox.hpp"

// triangle lvl
class Triangle: public Geg::Layer {
private:
public:
	Triangle() {

	}

	void onEvent(Geg::Event& event) override {
		Geg::Dispatcher dis(event);
	}

	void onUpdate(float deltaTime) override {
	}
};

// main app
sandboxApp::sandboxApp() {
	Triangle* triangleLayer = new Triangle();
	pushLayer(triangleLayer);
};
sandboxApp::~sandboxApp(){};

// main fn
Geg::App* Geg::createApp() {
	Geg::App* a = new sandboxApp();
	return a;
}
