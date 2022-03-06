#include "camera-controller.hpp"
#include "imgui.h"

CamController::CamController():
		cam(fov, aspectRatio, 1, -1) {
	const Geg::Window& win = Geg::App::get().getWindow();
	aspectRatio = win.getWidth() / win.getHeight();
	cam = Geg::Camera(fov, aspectRatio, 0.1f, 100);
}

void CamController::handleUpdates() {
	ImGui::Begin("cam");
	ImGui::SliderFloat3("translation", &postion[0], -100, 100);
	ImGui::End();

	cam.updateView(postion);
}
