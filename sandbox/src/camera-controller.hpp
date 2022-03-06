#pragma once

#include "geg.hpp"

class CamController {
public:
	CamController();

	void handleUpdates();
	const Geg::Camera getCam() const { return cam; }

private:
	glm::vec3 postion{1.f};
	float fov = 90;
	float aspectRatio = 0;

	Geg::Camera cam;
};