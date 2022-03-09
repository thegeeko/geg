#pragma once

#include "assets/mesh.hpp"
#include "camera-controller.hpp"
#include "geg.hpp"

class Level: public Geg::Layer {
private:
	Geg::Ref<Geg::Shader> shader;
	CamController camController;
	
	Geg::Ref<Geg::Mesh> mesh;
	Geg::Ref<Geg::Mesh> mesh1;

	glm::vec3 meshPos{0}, meshRotation, meshScale{1};
	glm::vec3 mesh1Pos, mesh1Rotation, mesh1Scale;
	
	Geg::Ref<Geg::Pipeline> pipeline;
	Geg::Ref<Geg::Pipeline> pipeline1;

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
	~sandboxApp() override;
};
