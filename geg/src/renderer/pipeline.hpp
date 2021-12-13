#pragma once

#include <memory>

#include "buffers.hpp"

namespace Geg {
	class Pipeline {
	public:
		virtual ~Pipeline() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static Pipeline* create(
				const Ref<VertexBuffer>&,
				const Ref<IndexBuffer>&,
				const Ref<Shader>&);
	};
}		 // namespace Geg
