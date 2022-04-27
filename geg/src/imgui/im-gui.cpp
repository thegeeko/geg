#include "im-gui.hpp"

#include <vulkan/vulkan.h>

#include "imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_vulkan.h"

namespace Geg {
	ImGuiLayer::ImGuiLayer() {
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());
		window = App::get().getWindow().getRawWindow();
		attach();
	}

	ImGuiLayer::~ImGuiLayer() {
		detach();
	}

	void ImGuiLayer::attach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		 // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		//		 Setup Dear ImGui style
		ImGui::StyleColorsLight();
		/* ImGui::StyleColorsClassic(); */

		initDescriptorPool();
		initRenderPass();

		QueueFamilyIndices qFams;
		context->device->findQueueFamilies(context->device->getPhysicalDevice(), qFams);

		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = context->device->getInstance();
		initInfo.PhysicalDevice = context->device->getPhysicalDevice();
		initInfo.Device = context->device->getDevice();
		initInfo.QueueFamily = qFams.graphicsFamily;
		initInfo.Queue = context->device->getGraphicsQueue();
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
		vkDestroyRenderPass(context->device->getDevice(), renderPass, nullptr);
		vkDestroyDescriptorPool(context->device->getDevice(), descriptorPool, nullptr);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::initDescriptorPool() {
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
	}

	void ImGuiLayer::initRenderPass() {
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = context->swapChain->getSwapChainImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult result = vkCreateRenderPass(context->device->getDevice(), &renderPassInfo, nullptr, &renderPass);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't craete render pass");
	}

	void ImGuiLayer::initFrame() {
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::render() {
		ImGuiIO& io = ImGui::GetIO();
		App& app = App::get();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		// Rendering
		ImGui::Render();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}		 // namespace Geg
