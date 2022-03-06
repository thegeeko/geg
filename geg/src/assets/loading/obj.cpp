#include "obj.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Geg::utils {
	Model loadObj(const std::string& filePath) {
		using namespace std;
		vector<float>* vertices = new vector<float>;
		vector<uint32_t>* indices = new vector<uint32_t>;
		BufferLayout layout;

		// tinyobj
		tinyobj::attrib_t attrib;
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		string err, warn;

		bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str());
		GEG_CORE_ASSERT(result, warn + " / " + err);

		layout.add(ShaderDataType::Float3, true);
		layout.add(ShaderDataType::Float3, true);

		for (auto& shape : shapes) {
			for (auto& index : shape.mesh.indices) {
				// position
				if (index.vertex_index >= 0) {
					float* vertcie = &attrib.vertices[3 * index.vertex_index];
					vertices->push_back(vertcie[0]);
					vertices->push_back(vertcie[1]);
					vertices->push_back(vertcie[2]);
				} else {
					vertices->push_back(0);
					vertices->push_back(0);
					vertices->push_back(0);
				}

				// normals
				if (index.normal_index >= 0) {
					float* normal = &attrib.normals[3 * index.normal_index];
					vertices->push_back(normal[0]);
					vertices->push_back(normal[1]);
					vertices->push_back(normal[2]);
				} else {
					vertices->push_back(0);
					vertices->push_back(0);
					vertices->push_back(0);
				}

				indices->push_back(indices->size());
			}
		}

		return Model(vertices, indices, layout);
	}
}		 // namespace Geg::utils