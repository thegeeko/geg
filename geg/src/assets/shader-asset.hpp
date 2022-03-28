#pragma once
#include "renderer/shader.hpp"

#include "shaderc/shaderc.hpp"

namespace Geg {
	struct ShaderSource {
		std::string vert;
		std::string frag;
	};
	
	class ShaderAsset {
	public:
		ShaderAsset() = default;
		ShaderAsset(const ShaderSource& src);
		size_t shaderHash;
		Ref<Shader> shader;
	private:
		std::vector<uint32_t> vertSpv;
		std::vector<uint32_t> fragSpv;

		std::vector<uint32_t> compile(const std::string& src, shaderc_shader_kind shader);
		void setup();
	};
} // namespace Geg