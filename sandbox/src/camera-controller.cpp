#include "camera-controller.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "imgui.h"

CamController::CamController():
		cam(fov, aspectRatio, 0.1f, 200.f) {
}

void CamController::update(float dt) {
	if (geg::Input::isKeyPressed(GEG_KEY_W)) {
		position.z += camSpeed * dt;
		recalculateView();
	}
	if (geg::Input::isKeyPressed(GEG_KEY_S)) {
		position.z -= camSpeed * dt;
		recalculateView();
	}
	if (geg::Input::isKeyPressed(GEG_KEY_A)) {
		position.x += camSpeed * dt;
		recalculateView();
	}
	if (geg::Input::isKeyPressed(GEG_KEY_D)) {
		position.x -= camSpeed * dt;
		recalculateView();
	}
	if (geg::Input::isKeyPressed(GEG_KEY_LEFT_CONTROL)) {
		position.y -= camSpeed * dt;
		recalculateView();
	}
	if (geg::Input::isKeyPressed(GEG_KEY_SPACE)) {
		position.y += camSpeed * dt;
		recalculateView();
	}
}

void CamController::drawUi() {
	ImGui::Begin("cam");

	ImGui::Text("Rotation :");
	if (ImGui::SliderAngle("X", &rotation.x)) {
		recalculateView();
	}
	if (ImGui::SliderAngle("Y", &rotation.y)) {
		recalculateView();
	}
	if (ImGui::SliderAngle("Z", &rotation.z)) {
		recalculateView();
	}

	ImGui::Separator();

	ImGui::SliderFloat("cam speed", &camSpeed, 0, 10);

	if (ImGui::DragFloat3("Cam Position", &position.x)) {
		recalculateView();
	}
	ImGui::End();
}

void CamController::recalculateView() {
	cam.view = glm::translate(glm::mat4(1.f), position);

	cam.view = glm::rotate(cam.view, rotation.x, glm::vec3(1.f, 0.f, 0.f));
	cam.view = glm::rotate(cam.view, rotation.y, glm::vec3(0.f, 1.f, 0.f));
	cam.view = glm::rotate(cam.view, rotation.z, glm::vec3(0.f, 0.f, 1.f));
}
