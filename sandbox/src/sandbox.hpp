#pragma once

#include "geg.hpp"
#include "camera-controller.hpp"
#include "ecs/entity.hpp"

class Level: public Geg::Layer {
private:
	CamController camController;

	std::string meshPath = std::string(" ", 256);
	std::string shaderPath = std::string(" ", 256);
	uint32_t activeId;

	Geg::Scene scene;
	Geg::Entity vase;
	Geg::Entity another;

	void drawComponents(Geg::Entity entity);

public:
	Level();
	void onEvent(Geg::Event& event) override;
	void onUpdate(float deltaTime) override;
	void onUiUpdate(float deltaTime) override;
};

class sandboxApp: public Geg::App {
	Level* lvl;

public:
	sandboxApp();
	~sandboxApp() override;
};
