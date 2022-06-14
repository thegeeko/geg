#pragma once

#include "geg.hpp"

class CamController {
public:
	CamController();

	void update(float dt);
	void drawUi();
	const geg::Camera getCam() const { return cam; }

private:
	float fov = 70;
	float aspectRatio = 1280.f / 720;

	geg::Camera cam;

	glm::vec3 position = {0.f, 0.f, -6.f};
	glm::vec3 rotation{0.f};
	float camSpeed = 6;

	void recalculateView();
};