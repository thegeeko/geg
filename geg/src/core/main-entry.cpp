#include "geg-pch.hpp"

extern Geg::App* Geg::createApp();

int main() {
	Geg::Logger::init();
	GEG_CORE_INFO("Hi :3");
	GEG_INFO("Hi :3");

	auto appIns = Geg::createApp();
	appIns->start();
	delete appIns;
}
