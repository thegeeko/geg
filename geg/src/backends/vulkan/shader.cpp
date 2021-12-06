#include "shader.hpp"

#include <vulkan/vulkan_handles.hpp>

#include "backends/vulkan/vulkan-device.hpp"

namespace Geg {
	VulkanShader::VulkanShader(std::string vertPath, std::string fragPath, GraphicsContext* _context) {
		context = dynamic_cast<VulkanGraphicsContext*>(_context);

		auto vertSrc = Geg::Utils::readFileAsBinary(vertPath);
		auto fragSrc = Geg::Utils::readFileAsBinary(fragPath);

		vertModule = createShaderModule(vertSrc);
		fragModule = createShaderModule(fragSrc);

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

		GEG_CORE_INFO("Shader moudules and stages created");
	}

	VulkanShader::~VulkanShader() {
		vkDestroyShaderModule(context->device->getDevice(), vertModule, nullptr);
		vkDestroyShaderModule(context->device->getDevice(), fragModule, nullptr);	
		GEG_CORE_INFO("Shader moudules and stages destroyed");
	}

	VkShaderModule VulkanShader::createShaderModule(const std::vector<char>& src) {
		VkShaderModule shaderModule;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = src.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(src.data());

		VkResult result = vkCreateShaderModule(context->device->getDevice(), &createInfo, nullptr, &shaderModule);
		GEG_ASSERT(result == VK_SUCCESS, "can't create a shader moudule");

		return shaderModule;
	}
	void VulkanShader::bind() const {};
	void VulkanShader::unBind() const {};
}		 // namespace Geg
