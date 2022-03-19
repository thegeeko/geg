#pragma once

#include "scene.hpp"
#include "entt.hpp"

namespace Geg {

	class Entity {
	public:
		Entity() = default;

		Entity(Scene* _scene, entt::entity _handle) :
			scene(_scene), entityHandle(_handle) {}

		~Entity() = default;

		template<typename T>
		[[nodiscard]] bool hasComponent() const {
			return scene->registry.any_of<T>(entityHandle);
		}

		template<typename T, typename... Args>
		void addComponent(Args&&... args) {
			GEG_CORE_ASSERT(!hasComponent<T>(), "This component already exist");
			scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() {
			GEG_CORE_ASSERT(hasComponent<T>(), "This component doesn't exist");
			return scene->registry.get<T>(entityHandle);
		}

	private:
		Scene* scene = nullptr;
		entt::entity entityHandle{entt::null};


	};
}
