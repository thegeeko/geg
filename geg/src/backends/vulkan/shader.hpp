#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "renderer/graphics-context.hpp"
#include "renderer/shader.hpp"

namespace Geg {
	class VulkanShader: public Shader {
	public:
		VulkanShader(std::string vertPath, std::string fragPath, GraphicsContext* _context);
		virtual ~VulkanShader();

		void bind() const override;
		void unBind() const override;

		const std::vector<VkPipelineShaderStageCreateInfo> getStages() const { return shaderStages; };

	private:
		VulkanGraphicsContext* context;

		VkShaderModule vertModule;
		VkShaderModule fragModule;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		VkShaderModule createShaderModule(const std::vector<char>& src);
	};
}		 // namespace Geg
