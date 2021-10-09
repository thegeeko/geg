#ifndef GEG_RENDERER_API_HPP
#define GEG_RENDERER_API_HPP

#include "glm/vec4.hpp"
#include "vertex-array.hpp"

namespace Geg {
	class RendererAPI {
	public:
		enum class API {
			None   = 0,
			OpenGL = 1,
			Vulkan = 2,
		};
		
		virtual void clear() = 0;
		virtual void clear(glm::vec4 color) = 0;
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)  = 0;


		static API i_API;
	};
}

#endif //GEG_RENDERER_API_HPP
