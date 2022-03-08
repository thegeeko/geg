#pragma once

#include "assets/mesh.hpp"
#include "camera-controller.hpp"
#include "geg.hpp"

class Level: public Geg::Layer {
private:
	Geg::Mesh mesh;
	Geg::Ref<Geg::Shader> shader;
	Geg::Ref<Geg::Pipeline> pipeline;
	CamController camController;

public:
	Level();
	void onEvent(Geg::Event& event) override;
	void onUpdate(float deltaTime) override;
	void onUiUpdate(float deltaTime) override;
};

class sandboxApp: public Geg::App {
private:
	Level* lvl;

public:
	sandboxApp();
	~sandboxApp();
};
