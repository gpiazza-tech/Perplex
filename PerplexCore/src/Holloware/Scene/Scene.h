#pragma once

#include "SceneHierarchy.h"
#include <Holloware/Core/UUID.h>

#include <entt.hpp>

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Holloware
{
	class Asset;
	class Entity;
	class EditorCamera;
	class Timestep;

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& name = std::string(), UUID uuid = UUID());
		Entity CreateAbstractEntity(const std::string& name = std::string(), UUID uuid = UUID());
		void DestroyEntity(Entity entity);
		void CopyEntity(Entity entity);
		Entity GetEntity(UUID uuid);

		void OnUpdateEditor(Timestep ts, const EditorCamera& camera);

		void OnStartRuntime();
		void OnUpdateRuntime(Timestep ts);
		void Scene::OnStopRuntime();

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnScriptAssetReimported(Asset asset);

		SceneHierarchy& GetHierarchy() { return m_Hierarchy; }
	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, entt::entity> m_UUIDMap;
		uint32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		SceneHierarchy m_Hierarchy;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}