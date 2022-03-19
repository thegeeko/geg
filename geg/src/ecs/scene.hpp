#pragma once

#include "entt.hpp"

namespace Geg {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		[[nodiscard]] Entity createEntity();
		[[nodiscard]] Entity createEntity(const std::string& name);
		[[nodiscard]] auto& getReg() { return registry; }
		void onUpdate(float deltaTime);

	private:
		entt::registry registry;
		friend class Entity;
	};
} // namespace Geg
