#pragma once

#include "SceneHierarchy.h"
#include <Perplex/Core/UUID.h>

#include <entt.hpp>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Perplex
{
	class Entity;
	class Component;

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		Entity CreateAbstractEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		void DestroyEntity(Entity entity);
		Entity CopyEntity(Entity entity, UUID parent = 0);
		Entity GetEntity(UUID uuid);
		std::vector<Entity> GetParentEntities();

		SceneHierarchy& GetHierarchy() { return m_Hierarchy; }

		template<typename T>
		auto View() { return m_Registry.view<T>(); }
	private:
		entt::registry m_Registry;

		std::unordered_map<UUID, entt::entity> m_UUIDMap;
		std::unordered_map<entt::entity, std::vector<Component>> m_ComponentsMap;

		uint32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		SceneHierarchy m_Hierarchy;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}