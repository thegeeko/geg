#pragma once

#include "entt.hpp"
#include "renderer/cams/perspective.hpp"

namespace geg {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		[[nodiscard]] Entity createEntity();
		[[nodiscard]] Entity createEntity(const std::string& name);
		[[nodiscard]] auto& getReg() { return registry; }
		void onUpdate(float deltaTime, Camera cam);

	private:
		entt::registry registry;
		friend class Entity;
	};
} // namespace geg
