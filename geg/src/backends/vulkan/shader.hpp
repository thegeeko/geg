#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "renderer/graphics-context.hpp"
#include "renderer/shader.hpp"

namespace Geg {
	class VulkanShader: public Shader {
	public:
		VulkanShader(std::string vertPath, std::string fragPath, GraphicsContext* _context);
		~VulkanShader();
		void bind() const override;
		void unBind() const override;

	private:
		VulkanGraphicsContext* context;

		VkShaderModule vertModule;
		VkShaderModule fragModule;

		VkShaderModule createShaderModule(const std::vector<char>& src);
	};
}		 // namespace Geg
