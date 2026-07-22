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

	struct DyingEntity
	{
		DyingEntity(UUID entityID, float time)
			: EntityID(entityID), Time(time) { }

		UUID EntityID{};
		float Time{};
	};

	void InitSceneSystems(Scene& scene);

	class Scene
	{
	public:
		Scene();
		Scene(const Scene& other);
		Scene(Scene&& other) = default;
		Scene& operator=(Scene other)
		{
			std::swap(m_Registry, other.m_Registry);
			std::swap(m_Hierarchy, other.m_Hierarchy);
			std::swap(m_UUIDMap, other.m_UUIDMap);
			std::swap(m_Systems, other.m_Systems);
			return *this;
		}
		Scene& operator=(Scene&& other) = default;

		Entity CreateEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		Entity CreateAbstractEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		void DestroyEntityNow(Entity entity);
		void DestroyEntity(Entity entity, float delay = 0.0f);
		Entity CopyEntity(Entity entity, UUID parent = 0);
		Entity GetEntity(UUID uuid);
		std::vector<Entity> GetParentEntities();

		void Start();
		void Update(Timestep ts);
		void Stop();

		bool IsPlaying() const { return m_Playing; }
		bool IsPaused() const { return m_Paused; }
		void SetPaused(bool paused) { m_Paused = paused; }

		float GetTimescale() const;
		void SetTimescale(float timescale);

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
		void RemoveEntity(Entity entity);
	private:
		bool m_Playing{ false };
		bool m_Paused{ false };
		float m_Timescale{ 1.0f };

		entt::registry m_Registry{};

		std::unordered_map<UUID, entt::entity> m_UUIDMap{};
		std::vector<DyingEntity> m_DyingEntities{};

		SceneHierarchy m_Hierarchy{};

		entt::entity m_SystemsContainer{};
		std::vector<SceneSystem*> m_Systems{};

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}