#include "camera-controller.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "imgui.h"

CamController::CamController() :
	cam(fov, aspectRatio, 0.1f, 100.f) {}

void CamController::handleUpdates() {
	ImGui::Begin("cam");
	if (ImGui::CollapsingHeader("Rotation")) {
		if (ImGui::SliderAngle("X", &rotation.x))
			cam.view = glm::rotate(cam.view, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 1.f));
		if (ImGui::SliderAngle("Y", &rotation.y))
			cam.view = glm::rotate(cam.view, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 1.f));
		if (ImGui::SliderAngle("Z", &rotation.z))
			cam.view = glm::rotate(cam.view, glm::radians(rotation.z), glm::vec3(1.f, 1.f, 0.f));
	}

	if (ImGui::SliderFloat3("Cam Position", &position.x, -1, 1)) {
		cam.view = glm::translate(original, position);
		
		cam.view = glm::rotate(cam.view, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 1.f));
		cam.view = glm::rotate(cam.view, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 1.f));
		cam.view = glm::rotate(cam.view, glm::radians(rotation.z), glm::vec3(1.f, 1.f, 0.f));
	}
	ImGui::End();

}
