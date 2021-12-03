#pragma once

#include <vulkan/vulkan.hpp>

#include "renderer/graphics-context.hpp"

namespace Geg {
	class VulkanGraphicsContext: public GraphicsContext {
	 public:
		VulkanGraphicsContext(GLFWwindow* window);
		virtual ~VulkanGraphicsContext() {};

		void init() override{};
		void swapBuffers() override{};

	 private:
	};
}		 // namespace Geg
