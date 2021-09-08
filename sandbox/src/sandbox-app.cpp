#include "sandbox-app.hpp"

sandboxApp::sandboxApp() {};
sandboxApp::~sandboxApp() {};

Geg::App* Geg::createApp() {
	return new sandboxApp();
}