#include "SceneHierarchyPanel.h"

#include <Holloware/Core/Core.h>
#include <Holloware/Core/UUID.h>
#include <Holloware/Scene/Entity.h>
#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Scene.h"

#include <imgui/imgui.h>
#include <cstdint>
#include <vector>
#include <Holloware/Scene/EntityNode.h>

namespace Holloware
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_SelectedNodes(0)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedNodes.clear();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		SceneHierarchy& hierarchy = m_Context->GetHierarchy();

		// Draw Nodes
		const std::vector<UUID>& rootEntities = hierarchy.GetRoot().ChildIDs;
		for (const auto& entityID : rootEntities)
			DrawEntityNode(hierarchy.GetNode(entityID));

		if ((ImGui::IsMouseDown(0) || ImGui::IsMouseDown(1)) && ImGui::IsWindowHovered() && !m_HoveredNode)
			m_SelectedNodes.clear();
		m_HoveredNode = 0;
		
		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity("Entity");
			if (ImGui::MenuItem("Create Abstract Entity"))
				m_Context->CreateAbstractEntity("Abstract Entity");
			if (m_CopiedNodes.size() > 0 && ImGui::MenuItem("Paste"))
			{
				UUID selectedNode = m_SelectedNodes.empty() ? 0 : m_SelectedNodes[0];
				for (auto& copiedNode : m_CopiedNodes)
					m_Context->CopyEntity(m_Context->GetEntity(copiedNode), selectedNode);
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedNodes.size() == 1)
		{
			Entity& selection = m_Context->GetEntity(m_SelectedNodes[0]);

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

	void SceneHierarchyPanel::SetSelectedEntity(UUID entity)
	{
		m_SelectedNodes.resize(1);
		m_SelectedNodes[0] = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(const EntityNode& node)
	{
		SceneHierarchy& hierarchy = m_Context->GetHierarchy();

		Entity& entity = m_Context->GetEntity(node.ID);
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		const std::vector<UUID>& childIDs = node.ChildIDs;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (childIDs.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		for (auto& selectedNode : m_SelectedNodes)
			if (hierarchy.GetNode(selectedNode).ID == node.ID)
				flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)node.ID, flags, tag.c_str());

		bool hovered = ImGui::IsItemHovered();
		if (hovered)
			m_HoveredNode = node.ID;

		// always selects on left click, selects on right click only when there is no current selection
		if (hovered && (ImGui::IsMouseReleased(0) || (ImGui::IsMouseReleased(1) && m_SelectedNodes.empty())))
		{
			if (ImGui::GetIO().KeyCtrl)
				m_SelectedNodes.emplace_back(node.ID);
			else
				SetSelectedEntity(node.ID);
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("SCENE_HIERARCHY_ITEM", (const void*)&entity, sizeof(Entity), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity payloadEntity = *(Entity*)payload->Data;
				if (payloadEntity != entity)
				{
					hierarchy.SetParent(payloadEntity.GetUUID(), entity.GetUUID());
				}
			}
			ImGui::EndDragDropTarget();
		}
		
		bool deleteEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				deleteEntity = true;
			if (ImGui::MenuItem("Copy"))
				m_CopiedNodes = m_SelectedNodes;

			ImGui::EndPopup();
		}

		if (opened)
		{
			// Draw child nodes
			for (auto& childID : childIDs)
				DrawEntityNode(hierarchy.GetNode(childID));

			ImGui::TreePop();
		}

		if (deleteEntity)
		{
			m_Context->DestroyEntity(entity);
			m_SelectedNodes.clear();
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