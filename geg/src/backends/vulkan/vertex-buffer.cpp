#include "vertex-buffer.hpp"

namespace geg {
	VulkanVertexBuffer::VulkanVertexBuffer(float* _vertices, unsigned int _size):
			context(dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext())),
			size(_size),
			vertices(_vertices) {
		createBufferAndAllocateMem();
		context->uploadDataToBuffer(vertices, static_cast<size_t>(size), bufferHandle);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer() {
		vmaDestroyBuffer(context->allocator, bufferHandle, allocationHandle);
	}

	void VulkanVertexBuffer::createBufferAndAllocateMem() {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = static_cast<size_t>(size);
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
											 VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkResult result = vmaCreateBuffer(
				context->allocator,
				&bufferInfo,
				&allocInfo,
				&bufferHandle,
				&allocationHandle,
				nullptr);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create vertex buffer");
		GEG_CORE_INFO("Created vertex buffer");
	}

	void VulkanVertexBuffer::setLayout(BufferLayout _layout) {
		layout = _layout;
		createInputBindingDescription();
		createInputAttributeDescription();
	}

	void VulkanVertexBuffer::createInputBindingDescription() {
		bindingDescription.binding = 0;
		bindingDescription.stride = layout.getStride();
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	}

	void VulkanVertexBuffer::createInputAttributeDescription() {
		attributeDescriptions.clear();
		uint32_t location = 0;
		for (const auto element : layout) {
			VkVertexInputAttributeDescription attribDesc;

			switch (element.type) {
				case ShaderDataType::None:
					GEG_CORE_ERROR("an element on vertex buffer layout has no type");
					break;
				case ShaderDataType::Float: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32_SFLOAT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}
				case ShaderDataType::Float2: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32_SFLOAT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}
				case ShaderDataType::Float3: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}
				case ShaderDataType::Float4: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}
				case ShaderDataType::Bool: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}

				case ShaderDataType::Int: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32_SINT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}

				case ShaderDataType::Int2: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32_SINT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}

				case ShaderDataType::Int3: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32_SINT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}

				case ShaderDataType::Int4: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32A32_SINT;
					attribDesc.offset = element.offset;
					location++;
					break;
				}
				case ShaderDataType::Mat3: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
					attribDesc.offset = element.offset;
					location += 3;
					break;
				}

				case ShaderDataType::Mat4: {
					attribDesc.binding = 0;
					attribDesc.location = location;
					attribDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
					attribDesc.offset = element.offset;
					location += 4;
					break;
				}
			}

			attributeDescriptions.push_back(attribDesc);
		}
	}

}		 // namespace geg
