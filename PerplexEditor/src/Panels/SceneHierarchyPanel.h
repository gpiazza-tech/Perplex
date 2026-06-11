#pragma once

#include <Perplex/Scene/Entity.h>
#include <Perplex/Core/Core.h>
#include <Perplex/ImGui/GuiSelection.h>

#include <vector>

namespace Perplex
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
		UUID GetSelectedEntity();
	private:
		void DrawEntityNode(const EntityNode& node);
		void DrawComponents(std::vector<Entity>& entitySelection);

		enum class DrawComponentStatus { None = 0, Removed };
		DrawComponentStatus DrawComponent(Component& component);
	
		void SetSelectedEntity(UUID entity);
	private:
		Ref<Scene> m_Context;
		UUID m_HoveredNode = 0;
		std::vector<UUID> m_SelectedNodes;
		std::vector<UUID> m_CopiedNodes;
	};
}