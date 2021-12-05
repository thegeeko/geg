#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/shader.hpp"
#include "renderer/renderer-api.hpp"

namespace Geg {
	class VulkanRendererAPI: public RendererAPI {
	public:
		/* VulkanRendererAPI(); */
		void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override {};
		void submit(const std::shared_ptr<Shader> &shader) override;
		void drawIndexed() override;
		void clear() override{};
		void clear(glm::vec4 color) override{};

	private:
		VkPipeline currentPipeline;
		VulkanGraphicsContext* vulkanContext;
		std::shared_ptr<VulkanShader> vulkanShader;

		void createPipeline(const std::shared_ptr<VulkanShader>& shader, VulkanGraphicsContext* context);
		void destroyPipeline(VulkanGraphicsContext* context);
	};
}		 // namespace Geg
