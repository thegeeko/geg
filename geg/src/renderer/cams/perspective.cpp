#include "perspective.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace Geg {
	Camera::Camera(float fov, float aspectRatio, float near, float far) {
    proj = glm::perspective(glm::radians(fov), aspectRatio, near, far);
    projView = proj * view;
	}

	void Camera::update() {
    projView = proj * view;
	}
}		 // namespace Geg