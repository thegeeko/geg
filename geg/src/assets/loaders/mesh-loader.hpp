#pragma once

#include "assets/mesh-asset.hpp"
#include "assimp/scene.h"

namespace geg {
	class MeshLoader {
	public:
		static MeshAsset loadModel(const std::string& path);

	private:
		MeshLoader() = default;
		
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		
		void processNodes(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);
	};
} // namespace geg
