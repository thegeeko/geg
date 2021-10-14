#pragma once

namespace Geg {

	enum class ShaderDataType {
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static int ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float: return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;
			case ShaderDataType::Mat3: return 4 * 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4 * 4;
			case ShaderDataType::Int: return 4;
			case ShaderDataType::Int2: return 4 * 2;
			case ShaderDataType::Int3: return 4 * 3;
			case ShaderDataType::Int4: return 4 * 4;
			case ShaderDataType::Bool: return 1;
			default: {
				GEG_CORE_ERROR("Unknown shader type");
				return 0;
			}
		}
	}

	class Shader {
	 public:
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		static Shader* create(std::string vertPath, std::string fragPath);
	};
}		 // namespace Geg
