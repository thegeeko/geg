#pragma once
#include "app.hpp"

#ifndef GEG_MAIN_ENTRY_HPP
#define GEG_MAIN_ENTRY_HPP


extern Geg::App* Geg::createApp();
int main () {

	Geg::Logger::init();
	GEG_CORE_WARN("Hi :3 {} {}", 32, 45);
	GEG_INFO("Hi :3");

	auto appIns = Geg::createApp();
	appIns->start();
	delete appIns;
}

#endif //GEG_MAIN_ENTRY_HPP
