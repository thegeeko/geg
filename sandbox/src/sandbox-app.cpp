#include "sandbox-app.hpp"

sandboxApp::sandboxApp() {};
sandboxApp::~sandboxApp() {};


Geg::App* Geg::createApp() {
	Geg::App* a = new sandboxApp();
	a->pushOverlay(new ImGuiLayer{});
	return a;
}