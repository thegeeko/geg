#include "mesh-loader.hpp"

#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "assimp/Importer.hpp"

namespace Geg {
	MeshAsset MeshLoader::loadModel(const std::string& path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
				path,
				aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			GEG_CORE_ERROR(" Error loadin model : ", importer.GetErrorString());
			return MeshAsset{};
		}

		MeshLoader loader;
		loader.processNodes(scene->mRootNode, scene);

		return MeshAsset(loader.vertices, loader.indices);
	}

	void MeshLoader::processNodes(aiNode* node, const aiScene* scene) {
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNodes(node->mChildren[i], scene);
		}
	}

	void MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			// position
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			// texture cords
			if (mesh->mTextureCoords[0]) {
				glm::vec2 vector;
				vector.x = mesh->mTextureCoords[0][i].x;
				vector.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = vector;
			} else
				vertex.texCoord = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		// indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
	}
} // namespace Geg
