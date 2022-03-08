#pragma once

#include "assets/mesh.hpp"
#include "camera-controller.hpp"
#include "geg.hpp"

class Level: public Geg::Layer {
private:
	Geg::Ref<Geg::VertexBuffer> vbo;
	Geg::Ref<Geg::IndexBuffer> ibo;
	Geg::Mesh mesh;
	Geg::Ref<Geg::Shader> shader;
	Geg::Ref<Geg::Pipeline> pipeline;
	Geg::Ref<Geg::Pipeline> pipeline1;
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
