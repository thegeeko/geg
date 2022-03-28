#pragma once

#include <vulkan/vulkan_core.h>

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/vertex-buffer.hpp"
#include "renderer/shader.hpp"

namespace Geg {
	class VulkanPipeline {
	public:
		VulkanPipeline(
				const Ref<VertexBuffer>& _vbo,
				const Ref<Shader>& _shader);
		~VulkanPipeline();

		const VkPipeline& getPipelineHandle() const { return pipelineHandle; };
		const VkPipelineLayout& getLayout() const { return layout; }

	private:
		VulkanGraphicsContext* context = nullptr;

		VkPipeline pipelineHandle = VK_NULL_HANDLE;
		VkPipelineLayout layout = VK_NULL_HANDLE;
	};
} // namespace Geg
