#pragma once
#include "assets/shader-asset.hpp"

namespace geg {
	class ShaderLoader {
	public:
		static ShaderAsset load(const std::string& path);
	private:
		ShaderLoader() = default;
		void processString(const std::vector<char>& fullSource);
		std::pair<uint32_t, uint32_t> vertexIndexes{0, 0}; // start - end
		std::pair<uint32_t, uint32_t> fragmentIndexes{0, 0}; // start - end
	};
} // namespace geg
