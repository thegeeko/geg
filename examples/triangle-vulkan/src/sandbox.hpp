#pragma once

#include "geg.hpp"

class Triangle: public Geg::Layer {
private:
	Geg::Ref<Geg::VertexBuffer> vbo;
	Geg::Ref<Geg::IndexBuffer> ibo;
	Geg::Ref<Geg::Shader> shader;
	Geg::Ref<Geg::Pipeline> pipeline;

public:
	Triangle();
	void onEvent(Geg::Event& event) override;
	void onUpdate(float deltaTime) override;
	void onUiUpdate() override;
};

class sandboxApp: public Geg::App {
private:
	Triangle* triangleLayer;

public:
	sandboxApp();
	~sandboxApp();
};
