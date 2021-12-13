#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/shader.hpp"
#include "backends/vulkan/vertex-buffer.hpp"
#include "renderer/renderer-api.hpp"

namespace Geg {
	class VulkanRendererAPI: public RendererAPI {
	public:
		/* VulkanRendererAPI(); */
		void init() override;
		void drawIndexed(const Ref<Pipeline>& _pipeline) override;
		void clear() override{};
		void clear(glm::vec4 color) override{};

	private:
		VkPipelineVertexInputStateCreateInfo vertexInfo{};
		VkPipeline currentPipeline;
		VulkanGraphicsContext* context = nullptr;
		size_t currentFrame = 0;
		bool a = true;

		void recordCommands();
		void beginRecording();
		void endRecording();
	};
}		 // namespace Geg
