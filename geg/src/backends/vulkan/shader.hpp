#pragma once

#include <spirv_reflect.h>
#include <vulkan/vulkan.h>

#include "backends/vulkan/graphics-context.hpp"
#include "renderer/graphics-context.hpp"
#include "renderer/shader.hpp"

namespace Geg {
	class VulkanShader: public Shader {
	private:
		VulkanGraphicsContext* context;

		struct ReflectionInfo {
			std::vector<VkDescriptorSetLayout> layouts;
		};

		VkShaderModule vertModule;
		VkShaderModule fragModule;
		SpvReflectShaderModule vertRefModule;		 // Spirv reflection module
		SpvReflectShaderModule fragRefModule;		 // spirv reflection module

		ReflectionInfo reflectionInfo;

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		VkShaderModule createShaderModule(const std::vector<char>& src);
		void reflectOn(SpvReflectShaderModule reflectModule);

	public:
		VulkanShader(std::string vertPath, std::string fragPath, GraphicsContext* _context);
		virtual ~VulkanShader();

		void bind() const override;
		void unBind() const override;

		const std::vector<VkPipelineShaderStageCreateInfo> getStages() const { return shaderStages; };
		const ReflectionInfo getReflectionInfo() const { return reflectionInfo; };
	};
}		 // namespace Geg
