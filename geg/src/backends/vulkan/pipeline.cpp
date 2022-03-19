#include "pipeline.hpp"

#include "shader.hpp"

namespace Geg {
	VulkanPipeline::VulkanPipeline(
			const Ref<VertexBuffer>& _vbo,
			const Ref<Shader>& _shader) {
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());

		auto shader = std::dynamic_pointer_cast<VulkanShader>(_shader);
		auto vbo = std::dynamic_pointer_cast<VulkanVertexBuffer>(_vbo);

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &vbo->getBindingDesc();
		vertexInputInfo.vertexAttributeDescriptionCount =
				static_cast<uint32_t>(vbo->getAttribDesc().size());
		vertexInputInfo.pVertexAttributeDescriptions = vbo->getAttribDesc().data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(context->swapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(context->swapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = context->swapChain->getSwapChainExtent();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;		// Optional
		rasterizer.depthBiasClamp = 0.0f;		 // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f;		 // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;		// Optional
		multisampling.pSampleMask = nullptr;		// Optional
		multisampling.alphaToCoverageEnable = VK_FALSE;		 // Optional
		multisampling.alphaToOneEnable = VK_FALSE;		// Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT |
				VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT |
				VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;		 // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;		// Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;		// Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;		 // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;		// Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;		// Optional
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;		 // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;		 // Optional
		colorBlending.blendConstants[1] = 0.0f;		 // Optional
		colorBlending.blendConstants[2] = 0.0f;		 // Optional
		colorBlending.blendConstants[3] = 0.0f;		 // Optional

		VkDynamicState dynamicStates[2] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_LINE_WIDTH};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		auto setsInfo = shader->getReflectionInfo();

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRange.size = ShaderDataTypeSize(ShaderDataType::Mat4);
		pushConstantRange.offset = 0;
		
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = setsInfo.layouts.size();		// Optional
		pipelineLayoutInfo.pSetLayouts = setsInfo.layouts.data();		 // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 1;		// Optional
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;		 // Optional

		VkResult result = vkCreatePipelineLayout(
				context->device->getDevice(),
				&pipelineLayoutInfo,
				nullptr,
				&layout);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create pipline layout");

		
		VkPipelineShaderStageCreateInfo shaderStages[] = {
				shader->getStages()[0],
				shader->getStages()[1]};
		
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shader->getStages().size();
		pipelineInfo.pStages = shaderStages; 
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;		// Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;		 // Optional
		pipelineInfo.layout = layout;
		pipelineInfo.renderPass = context->swapChain->getRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;		 // Optional
		pipelineInfo.basePipelineIndex = -1;		// Optional

		result = vkCreateGraphicsPipelines(
				context->device->getDevice(),
				VK_NULL_HANDLE,
				1,
				&pipelineInfo,
				nullptr,
				&pipelineHandle);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create pipeline");
	}

	VulkanPipeline::~VulkanPipeline() {
		vkDestroyPipeline(context->device->getDevice(), pipelineHandle, nullptr);
		vkDestroyPipelineLayout(context->device->getDevice(), layout, nullptr);
		// GEG_CORE_INFO("Pipeline layout destroyed");
	}
}		 // namespace Geg
