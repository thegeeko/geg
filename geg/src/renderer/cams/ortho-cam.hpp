#pragma once

#include "glm/glm.hpp"

namespace Geg {

	class OrthoCam {
	 public:
		OrthoCam(float top, float bottom, float left, float right);

		void setPosition(glm::vec3 pos) {
			position = pos;
			updateM();
		}
		void setRotation(float zAxisRotation) {
			rotation = zAxisRotation;
			updateM();
		}

		const glm::mat4& getProjM() const { return projM; }
		const glm::mat4& getViewM() const { return viewM; }
		const glm::mat4& getProjVeiwM() const { return projViewM; }

	 private:
		glm::mat4 projM;
		glm::mat4 viewM;
		glm::mat4 projViewM;

		glm::vec3 position;
		float rotation;

		void updateM();
	};
}		 // namespace Geg
