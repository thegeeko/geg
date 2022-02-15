#include "im-gui.hpp"

#include <vulkan/vulkan_core.h>

#include "core/logger.hpp"
#include "imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_vulkan.h"

namespace Geg {
	ImGuiLayer::ImGuiLayer(){
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());
		window = App::get().getWindow().getRawWindow();

		attach();
	}

	ImGuiLayer::~ImGuiLayer(){
		detach();
	};

	void ImGuiLayer::attach() {
		VkDescriptorPoolSize poolSizes[] = {
				{VK_DESCRIPTOR_TYPE_SAMPLER, 2000},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2000},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2000},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 2000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 2000},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 2000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 2000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 2000},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 2000}};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 2000 * IM_ARRAYSIZE(poolSizes);
		poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;

		VkResult result = vkCreateDescriptorPool(
				context->device->getDevice(),
				&poolInfo,
				nullptr,
				&descriptorPool);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create descriptor pool for imgui");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsLight();
		/* ImGui::StyleColorsClassic(); */

		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = context->device->getInstance();
		initInfo.PhysicalDevice = context->device->getPhysicalDevice();
		initInfo.Device = context->device->getDevice();

		QueueFamilyIndices qFams;
		context->device->findQueueFamilies(context->device->getPhysicalDevice(), qFams);

		initInfo.QueueFamily = qFams.graphicsFamily;
		initInfo.Queue = context->device->getGraphicsQueue();

		// pipeline cache is a potential future optimization, ignoring for now
		initInfo.PipelineCache = VK_NULL_HANDLE;
		initInfo.DescriptorPool = descriptorPool;
		initInfo.Allocator = VK_NULL_HANDLE;
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = context->swapChain->getSwapChainImages().size();
		initInfo.CheckVkResultFn = checkVulkanErrors;

		ImGui_ImplVulkan_Init(&initInfo, context->swapChain->getRenderPass());

		// upload fonts, this is done by recording and submitting a one time use command buffer
		// which can be done easily bye using some existing helper functions on the lve device object
		auto commandBuffer = VulkanGraphicsContext::beginSingleTimeCommand();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		VulkanGraphicsContext::endSingleTimeCommand(commandBuffer);
		ImGui_ImplVulkan_DestroyFontUploadObjects();

		GEG_CORE_INFO("Imgui layer attached");
	}

	void ImGuiLayer::detach() {
		GEG_CORE_INFO("Detaching Imgui");
		vkDestroyDescriptorPool(context->device->getDevice(), descriptorPool, nullptr);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void ImGuiLayer::init() {
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();	
	}

	void ImGuiLayer::render() {
		ImGuiIO& io = ImGui::GetIO();
		App& app = App::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		// Rendering
		ImGui::Render();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}		 // namespace Geg
