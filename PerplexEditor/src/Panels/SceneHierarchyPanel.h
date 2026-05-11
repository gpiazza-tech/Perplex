#pragma once

#include "Holloware/Scene/Entity.h"
#include "Holloware/Core/Core.h"

#include <vector>

namespace Holloware
{
	class Scene;
	class Component;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(const EntityNode& node);
		void DrawComponents(Entity entity);

		enum class DrawComponentStatus { None = 0, Removed };
		DrawComponentStatus DrawComponent(Entity entity, Component& component);
	
		void SetSelectedEntity(UUID entity);
	private:
		Ref<Scene> m_Context;
		UUID m_HoveredNode = 0;
		std::vector<UUID> m_SelectedNodes;
		std::vector<UUID> m_CopiedNodes;
	};
}