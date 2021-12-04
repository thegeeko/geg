#include "core.hpp"

#include <fstream>
#include <iostream>

namespace Geg::Utils {
	std::string readFileAsString(const std::string &filePath) {
		std::ifstream file{filePath, std::ios::ate};

		GEG_CORE_ASSERT(file.is_open(), "can't open file" + filePath);

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		std::string stringBuffer(buffer.begin(), buffer.end());
		return stringBuffer;
	}

	std::vector<char> readFileAsBinary(const std::string &filePath) {
		std::ifstream file{filePath, std::ios::ate | std::ios::binary};

		GEG_CORE_ASSERT(file.is_open(), "can't open file" + filePath);

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;;
	}

}		 // namespace Geg::Utils
