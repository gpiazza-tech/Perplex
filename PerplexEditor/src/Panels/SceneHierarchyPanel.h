#pragma once

#include "Holloware/Scene/Entity.h"

#include "Holloware/Core/Core.h"

namespace Holloware
{
	class Scene;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext[0]; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template <typename T>
		void DrawComponent(Entity entity, const char* name, void (*DrawBody)(T&));
	private:
		Ref<Scene> m_Context;
		std::vector<Entity> m_SelectionContext;
		std::vector<Entity> m_Clipboard;
	};
}