#pragma once

#include "geg.hpp"

class CamController {
public:
	CamController();

	void handleUpdates();
	const Geg::Camera getCam() const { return cam; }

private:
	glm::vec3 position{0.f};
	float fov = 90;
	float aspectRatio = 0;
	glm::vec3 rotation{0.f};

	float zoom = 0; 
	glm::mat4 original{1.f};

	Geg::Camera cam;
};