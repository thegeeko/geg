#include "shader.hpp"

#include <vulkan/vulkan_core.h>

#include "backends/vulkan/descriptors.hpp"
#include "backends/vulkan/vulkan-device.hpp"
#include "core/core.hpp"
#include "spirv_reflect.h"

namespace Geg {
	VulkanShader::VulkanShader(std::vector<uint32_t> vertSrc, std::vector<uint32_t> fragSrc) {
		GraphicsContext* _context = App::get().getWindow().getGraphicsContext();
		context = dynamic_cast<VulkanGraphicsContext*>(_context);

		vertModule = createShaderModule(vertSrc);
		fragModule = createShaderModule(fragSrc);

		SpvReflectResult result = spvReflectCreateShaderModule(vertSrc.size() * sizeof(vertSrc[0]), vertSrc.data(), &vertRefModule);
		GEG_CORE_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Can't Reflect on a shader");

		result = spvReflectCreateShaderModule(fragSrc.size() * sizeof(fragSrc[0]), fragSrc.data(), &fragRefModule);
		GEG_CORE_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Can't Reflect on a shader");

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragModule;
		fragShaderStageInfo.pName = "main";

		shaderStages.push_back(vertShaderStageInfo);
		shaderStages.push_back(fragShaderStageInfo);

		reflectOn(vertRefModule);

		GEG_CORE_INFO("Shader modules and stages created");
	}

	VulkanShader::~VulkanShader() {
		vkDestroyShaderModule(context->device->getDevice(), vertModule, nullptr);
		vkDestroyShaderModule(context->device->getDevice(), fragModule, nullptr);
		GEG_CORE_INFO("Shader modules and stages destroyed");
	}

	VkShaderModule VulkanShader::createShaderModule(const std::vector<uint32_t>& src) {
		VkShaderModule shaderModule;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = src.size() * sizeof(src[0]);
		createInfo.pCode = src.data();

		VkResult result = vkCreateShaderModule(context->device->getDevice(), &createInfo, nullptr, &shaderModule);
		GEG_ASSERT(result == VK_SUCCESS, "Can't create a shader module");

		return shaderModule;
	}

	void VulkanShader::reflectOn(SpvReflectShaderModule reflectModule) {
		for (int i = 0; i < reflectModule.descriptor_set_count; i++) {
			auto builder = DescriptorBuilder::begin(context->descriptorLayoutCache, context->descriptorsAlloc);
			for (int j = 0; j < reflectModule.descriptor_sets[i].binding_count; j++) {
				VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				VkShaderStageFlagBits stage = VK_SHADER_STAGE_ALL_GRAPHICS;
				builder.bindBuffer(j, type, stage);
			}
			VkDescriptorSetLayout layout{};
			builder.build(layout);
			reflectionInfo.layouts.push_back(layout);
		}
	};

	void VulkanShader::bind() const {};
	void VulkanShader::unBind() const {};
}		 // namespace Geg
