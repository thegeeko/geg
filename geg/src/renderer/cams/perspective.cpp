#include "perspective.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace Geg {
	Camera::Camera(float fov, float aspectRatio, float near, float far) {
    updateProjection(fov, aspectRatio, near, far);
		view = glm::rotate(view, glm::radians(45.f), glm::vec3(0.f, 0.f, 1.f));
    projView = proj * view;
	}

	void Camera::updateView(glm::vec3 tranlate) {
    // view = glm::translate(view, tranlate);
    projView = proj * view;
	}

	void Camera::updateProjection(float fov, float aspectRatio, float near, float far) {
    proj = glm::perspective(glm::radians(fov), aspectRatio, near, far);
    projView = proj * view;
	}

}		 // namespace Geg