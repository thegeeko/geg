#pragma once

#include "assets/mesh.hpp"
#include "assimp/scene.h"

namespace Geg {
	class Loader {
	public:
		static Mesh* loadModel(const std::string& path);
		std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

	private:
		void processNodes(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);
	};
}		 // namespace Geg