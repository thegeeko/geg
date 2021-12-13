#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/index-buffer.hpp"
#include "backends/vulkan/shader.hpp"
#include "backends/vulkan/vertex-buffer.hpp"
#include "renderer/pipeline.hpp"
#include "renderer/shader.hpp"

namespace Geg {
	class VulkanPipeline: public Pipeline {
	public:
		VulkanPipeline(
				const Ref<VertexBuffer>& _vbo,
				const Ref<IndexBuffer>& _ibo,
				const Ref<Shader>& _shader);
		~VulkanPipeline();

		void bind() const override{};
		void unbind() const override{};
		const Ref<VulkanVertexBuffer>& getVbo() const { return vbo; };
		const Ref<VulkanIndexBuffer>& getIbo() const { return ibo; };
		const Ref<VulkanShader>& getShader() const { return shader; };
		const VkPipeline& getPipelineHandle() const { return pipelineHandle; };

	private:
		VulkanGraphicsContext* context;

		VkPipeline pipelineHandle;
		VkPipelineLayout layout;
		Ref<VulkanVertexBuffer> vbo;
		Ref<VulkanIndexBuffer> ibo;
		Ref<VulkanShader> shader;
	};
}		 // namespace Geg
