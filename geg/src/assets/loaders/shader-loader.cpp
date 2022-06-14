#include "shader-loader.hpp"

namespace geg {

	const std::string VERTEX_FLAG = "vertex";
	const std::string FRAGMENT_FLAG = "fragment";
	const std::string GEG_FLAG = "###"; // to identify geg's flags

	ShaderAsset ShaderLoader::load(const std::string& path) {
		const std::vector<char> fullSource = Utils::readFileAsBinary(path.c_str());

		ShaderLoader loader{};
		loader.processString(fullSource);

		const char* str = fullSource.data();

		size_t size = loader.vertexIndexes.second - loader.vertexIndexes.first;
		const std::string vertSource(str + loader.vertexIndexes.first, size);

		size = loader.fragmentIndexes.second - loader.fragmentIndexes.first;
		const std::string fragSource(str + loader.fragmentIndexes.first, size);

		ShaderSource src{};
		src.vert = vertSource;
		src.frag = fragSource;

		return src;
	}

	void ShaderLoader::processString(const std::vector<char>& fullSource) {
		auto flagCounter = GEG_FLAG.size();
		std::string lastFound;

		uint32_t i = 0;
		for (; i < fullSource.size(); i++) {
			auto& chr = fullSource[i];

			if (chr == GEG_FLAG[0])
				flagCounter--; // it's # check if they match the flag length
			else
				flagCounter = GEG_FLAG.size();

			if (flagCounter == 0) {
				std::string flag;
				// will the string after the ### to check if it's a geg flag
				for (uint32_t j = i + 1; i < fullSource.size(); j++) {
					if (fullSource[j] == '\n') break;
					flag.push_back(fullSource[j]);
				}

				// it's a vertex flag
				if (flag == VERTEX_FLAG) {
					if (lastFound == VERTEX_FLAG) {
						vertexIndexes.second = i - 3;
						lastFound = "";
					}
					if (lastFound == FRAGMENT_FLAG) {
						fragmentIndexes.second = i - 3;
						lastFound = "";
					}
					i += flag.size();
					vertexIndexes.first = i + 1;
					lastFound = VERTEX_FLAG;
				}

				// it's a fragment flag
				if (flag == FRAGMENT_FLAG) {
					if (lastFound == VERTEX_FLAG) {
						vertexIndexes.second = i - 3;
						lastFound = "";
					}
					if (lastFound == FRAGMENT_FLAG) {
						fragmentIndexes.second = i - 3;
						lastFound = "";
					}
					i += flag.size();
					fragmentIndexes.first = i + 1;
					lastFound = FRAGMENT_FLAG;
				}
			}
		}

		if (lastFound == VERTEX_FLAG) {
			vertexIndexes.second = i;
			lastFound = "";
		}
		if (lastFound == FRAGMENT_FLAG) {
			fragmentIndexes.second = i;
			lastFound = "";
		}
	}
}
