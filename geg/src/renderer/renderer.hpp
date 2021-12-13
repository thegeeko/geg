#pragma once

#include <memory.h>

#include "renderer-api.hpp"
#include "renderer-commands.hpp"

namespace Geg {
	struct Renderer {
		static void beginScene();
		static void endScene();
		static void submit(const Ref<Pipeline>& pipeline);
		static inline GraphicsAPI getAPI() { return RendererCommands::getAPI(); }
	};
}		 // namespace Geg
