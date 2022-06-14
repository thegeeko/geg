#include "geg-pch.hpp"

extern geg::App* geg::createApp();

int main() {
	geg::Logger::init();
	GEG_CORE_INFO("Hi :3");
	GEG_INFO("Hi :3");

	auto appIns = geg::createApp();
	appIns->start();
	delete appIns;
}
