#include "sandbox-app.hpp"

sandboxApp::sandboxApp() {};
sandboxApp::~sandboxApp() {};


Geg::App* Geg::createApp() {
	Geg::App* a = new sandboxApp();
	return a;
}