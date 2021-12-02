#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

#include "GLFW/glfw3.h"
#include "renderer/graphics-context.hpp"

namespace Geg {
	class VulkanDevice {
	public:
		VulkanDevice(GLFWwindow* _window);
		~VulkanDevice();

		const VkDevice& getDevice() const { return device; }
		const VkQueue& getGraphicsQueue() const { return graphicsQueue; }
		const VkQueue& getPresentQueue() const { return presentQueue; }

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData,
				void* pUserData) {
			if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
				GEG_CORE_ERROR(_CallbackData->pMessage);
			} else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
				GEG_CORE_WARN(_CallbackData->pMessage);
			} else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
				GEG_CORE_INFO(_CallbackData->pMessage);
			} else {
				GEG_CORE_TRACE(_CallbackData->pMessage);
			}
			return VK_FALSE;
		}

	private:
		GLFWwindow* window;
		VkInstance vulkanInstance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
		};

		struct QueueFamilyIndices {
			unsigned int graphicsFamily;
			unsigned int presentFamily;

			bool vaiable = false;
		};

		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void populateDebugMessengerCreationInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void pickPhysicalDevice();
		void findQueueFamilies(const VkPhysicalDevice& physicalDevice, QueueFamilyIndices& queue);
	};
}		 // namespace Geg
