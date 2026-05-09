#pragma once

#include "SceneHierarchy.h"
#include <Holloware/Core/UUID.h>

#include <entt.hpp>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Holloware
{
	class Asset;
	class Entity;
	class EditorCamera;
	class Timestep;
	class Component;

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		Entity CreateAbstractEntity(const std::string& name = std::string(), UUID uuid = UUID(), UUID parent = 0);
		void DestroyEntity(Entity entity);
		void CopyEntity(Entity entity, UUID parent = 0);
		Entity GetEntity(UUID uuid);

		void OnUpdateEditor(Timestep ts, const EditorCamera& camera);

		void OnStartRuntime();
		void OnUpdateRuntime(Timestep ts);
		void OnStopRuntime();

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnScriptAssetReimported(Asset asset);

		SceneHierarchy& GetHierarchy() { return m_Hierarchy; }
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