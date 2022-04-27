#pragma once

#include <spirv_reflect.h>
#include <vulkan/vulkan.h>

#include "backends/vulkan/graphics-context.hpp"
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

		VkShaderModule createShaderModule(const std::vector<uint32_t>& src);
		void reflectOn(SpvReflectShaderModule reflectModule);

	public:
		VulkanShader(std::vector<uint32_t> vertSrc, std::vector<uint32_t> fragSrc);
		virtual ~VulkanShader();

		void bind() const override;
		void unBind() const override;

		const std::vector<VkPipelineShaderStageCreateInfo> getStages() const { return shaderStages; };
		const ReflectionInfo& getReflectionInfo() const { return reflectionInfo; };
	};
}		 // namespace Geg
