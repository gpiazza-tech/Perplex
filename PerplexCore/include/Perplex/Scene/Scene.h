#pragma once

#include "SceneHierarchy.h"
#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Core.h>
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
		Scene();

		Entity CreateEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		Entity CreateAbstractEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		void DestroyEntity(Entity entity);
		void DestroyEntityDelay(Entity entity, float delay);
		Entity CopyEntity(Entity entity, UUID parent = 0);
		Entity GetEntity(UUID uuid);
		std::vector<Entity> GetParentEntities();

		void Start();
		void Update(Timestep ts);
		void Stop();

		SceneHierarchy& GetHierarchy() { return m_Hierarchy; }

		template<typename T>
		auto View() { return m_Registry.view<T>(); }

		template<typename T>
		void AddSystem()
		{
			T* systemPtr = new T(Ref<Scene>(this));

			m_Systems.emplace_back((SceneSystem*)systemPtr);
			m_Registry.emplace<T*>(m_SystemsContainer, systemPtr);
		} 

		template<typename T>
		T& GetSystem()
		{
			T* systemPtr = m_Registry.get<T*>(m_SystemsContainer);
			return *systemPtr;
		}
	private:
		Entity ConstructEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
	private:
		entt::registry m_Registry;

		std::unordered_map<UUID, entt::entity> m_UUIDMap;
		std::unordered_map<entt::entity, std::vector<Component>> m_ComponentsMap;
		std::vector<DyingEntity> m_DyingEntities;

		SceneHierarchy m_Hierarchy;

		entt::entity m_SystemsContainer{};
		std::vector<SceneSystem*> m_Systems{};

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}