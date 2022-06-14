#pragma once

namespace geg {
	class Texture {
	public:
		virtual ~Texture() = default;
		virtual void* getHandler() = 0;

		static Texture* create(const std::string& path);
	};
}		 // namespace geg