#pragma once

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.hpp>

#include "GLFW/glfw3.h"
#include "renderer/graphics-context.hpp"

namespace Geg {
	struct QueueFamilyIndices {
		unsigned int graphicsFamily;
		unsigned int presentFamily;

		bool vaiable = false;
	};

	class VulkanDevice {
	public:
		VulkanDevice(GLFWwindow* _window);
		~VulkanDevice();

		const VkDevice& getDevice() const { return device; }
		const VkInstance& getInstance() const { return vulkanInstance; }
		const VkPhysicalDevice& getPhysicalDevice() const { return physicalDevice; }
		const VkPhysicalDeviceProperties& getGpuProperties() const { return gpuProperties; }
		const VkSurfaceKHR& getSurface() const { return surface; }
		const VkQueue& getGraphicsQueue() const { return graphicsQueue; }
		const VkQueue& getPresentQueue() const { return presentQueue; }

		void findQueueFamilies(const VkPhysicalDevice& physicalDevice, QueueFamilyIndices& queue) const;

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
		VkPhysicalDevice physicalDevice;
		VkPhysicalDeviceProperties gpuProperties;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME};

		bool checkValidationLayerSupport();
		bool checkDeviceExtentionSupport(VkPhysicalDevice device);
		std::vector<const char*> getRequiredExtensions();
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void populateDebugMessengerCreationInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void pickPhysicalDevice();
	};
}		 // namespace Geg
