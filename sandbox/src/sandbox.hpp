#pragma once

#include "geg.hpp"
#include "camera-controller.hpp"
#include "ecs/entity.hpp"

class Level: public geg::Layer {
private:
	CamController camController;

	std::string meshPath = std::string(" ", 256);
	std::string shaderPath = std::string(" ", 256);
	uint32_t activeId;

	geg::Scene scene;
	geg::Entity vase;
	geg::Entity light;
	geg::Entity light2;
	geg::Entity floor;

	void drawComponents(geg::Entity entity);

public:
	Level();
	void onEvent(geg::Event& event) override;
	void onUpdate(float deltaTime) override;
	void onUiUpdate(float deltaTime) override;
};

class sandboxApp: public geg::App {
	Level* lvl;

public:
	sandboxApp();
	~sandboxApp() override;
};
