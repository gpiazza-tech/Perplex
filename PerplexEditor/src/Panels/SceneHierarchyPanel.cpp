#include "SceneHierarchyPanel.h"

#include "Holloware/ImGui/ImGuiUtilities.h"

#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Scene.h"

#include <entt.hpp>
#include <imgui/imgui_internal.h>

namespace Holloware
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_SelectionContext(0)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext.clear();
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// Sort before drawing
		/*m_Context->m_Registry.sort<entt::entity>([](const entt::entity lhs, const entt::entity rhs) {
			return entt::registry::entity(lhs) < entt::registry::entity(rhs);
			});*/

		auto& view = m_Context->m_Registry.view<entt::entity>();

		// TODO: reverse draw order
		for (auto entityID : m_Context->m_Registry.view<entt::entity>())
		{
			Entity entity = Entity(entityID, m_Context.get());
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext.clear();
		
		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity("Entity");
			if (ImGui::MenuItem("Create Abstract Entity"))
				m_Context->CreateAbstractEntity("Abstract Entity");
			if (m_Clipboard.size() > 0 && ImGui::MenuItem("Paste"))
				m_Context->CopyEntity(m_Clipboard[0]);

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext.size() == 1)
		{
			Entity& selection = m_SelectionContext[0];

			DrawComponents(selection);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!selection.HasComponent<TransformComponent>() && ImGui::MenuItem("Transform"))
				{
					selection.AddComponent<TransformComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!selection.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
				{
					selection.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!selection.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
				{
					selection.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!selection.HasComponent<ScriptComponent>() && ImGui::MenuItem("Script"))
				{
					selection.AddComponent<ScriptComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext.resize(1);
		m_SelectionContext[0] = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		for (auto& selection : m_SelectionContext)
			if (selection == entity)
				flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemHovered() && (ImGui::IsMouseReleased(0) || ImGui::IsMouseReleased(1))) // selects on both left and right click
		{
			SetSelectedEntity(entity);
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("SCENE_HIERARCHY_ITEM", (const void*)&entity, sizeof(Entity), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
		
		bool deleteEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				deleteEntity = true;
			if (ImGui::MenuItem("Copy"))
				m_Clipboard = m_SelectionContext;

			ImGui::EndPopup();
		}

		if (opened)
		{
			DrawEntityNode(entity);
			ImGui::TreePop();
		}

		if (deleteEntity)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext[0] == entity)
				m_SelectionContext.erase(m_SelectionContext.begin());
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<IDComponent>())
		{
			auto& id = entity.GetComponent<IDComponent>();
			id.DrawGui();
		}

		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();
			tagComponent.DrawGui();
		}

		DrawComponent<TransformComponent>(entity, "Transform", [](TransformComponent& c)
			{
				c.DrawGui();
			});

		DrawComponent<CameraComponent>(entity, "Camera", [](CameraComponent& c)
			{
				c.DrawGui();
			});

		DrawComponent<SpriteRendererComponent>(entity, "Sprite Renderer", [](SpriteRendererComponent& c)
			{
				c.DrawGui();
			});

		DrawComponent<ScriptComponent>(entity, "Script", [](ScriptComponent& c)
			{
				c.DrawGui();
			});
	}

	template <typename T>
	void SceneHierarchyPanel::DrawComponent(Entity entity, const char* name, void (*DrawBody)(T&))
	{
		if (entity.HasComponent<T>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap, name);
			bool removeComponent = false;

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Remove"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				auto& c = entity.GetComponent<T>();
				DrawBody(c);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}
}