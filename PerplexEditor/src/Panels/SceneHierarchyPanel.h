#pragma once

#include <Perplex/Scene/Entity.h>
#include <Perplex/Core/Core.h>

#include <vector>

namespace Perplex
{
	class Scene;
	class Component;

	class SceneHierarchyPanel
	{
	public:
		void OnImGuiRender();
		UUID GetSelectedEntity();
	public:
		enum class DrawComponentStatus { None = 0, Removed };
	private:
		void DrawEntityNode(const EntityNode& node);
		void DrawComponents(std::vector<Entity>& entitySelection);

		void SetSelectedEntity(UUID entity);
	private:
		UUID m_HoveredNode{};
		std::vector<UUID> m_SelectedNodes{};
		std::vector<UUID> m_CopiedNodes{};
	};
}