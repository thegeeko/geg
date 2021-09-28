#ifndef GEG_RENDERER_API_HPP
#define GEG_RENDERER_API_HPP

namespace Geg {
	class RendererAPI {
	public:
		enum class API {
			None   = 0,
			OpenGL = 1,
			Vulkan = 2,
		};

		static API getApi() {return i_API;}
	private:

		static API i_API;
	};
}

#endif //GEG_RENDERER_API_HPP
