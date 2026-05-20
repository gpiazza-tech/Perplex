#pragma once

#include "SceneHierarchy.h"
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Timestep.h>

#include <entt.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace Perplex
{
	class Entity;
	class Component;

	struct DyingEntity
	{
		DyingEntity(UUID entityID, float time)
			: EntityID(entityID), Time(time) { }

		UUID EntityID{};
		float Time{};
	};

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		Entity CreateAbstractEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		void DestroyEntity(Entity entity);
		void DestroyEntityDelay(Entity entity, float delay);
		Entity CopyEntity(Entity entity, UUID parent = 0);
		Entity GetEntity(UUID uuid);
		std::vector<Entity> GetParentEntities();

		void Update(Timestep ts);

		SceneHierarchy& GetHierarchy() { return m_Hierarchy; }

		template<typename T>
		auto View() { return m_Registry.view<T>(); }
	private:
		entt::registry m_Registry;

		std::unordered_map<UUID, entt::entity> m_UUIDMap;
		std::unordered_map<entt::entity, std::vector<Component>> m_ComponentsMap;
		std::vector<DyingEntity> m_DyingEntities;

		SceneHierarchy m_Hierarchy;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}