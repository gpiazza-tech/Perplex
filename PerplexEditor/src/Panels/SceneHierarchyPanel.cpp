#include <pch.h>
#include "SceneHierarchyPanel.h"

#include <Perplex/Perplex.h>
#include <Perplex/ImGui/GuiSelection.h>
#include <Perplex/ImGui/PrimitiveDrawers.h>
#include <Perplex/Components/ComponentDrawers.h>
#include <Perplex/Components/ComponentKind.h>
#include <Perplex/Components/ComponentRegistry.h>
#include <Perplex/Scene/SceneManager.h>

#include <imgui/imgui.h>
#include <cstdint>
#include <vector>
#include <string>

namespace Perplex
{
	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (SceneManager::Get().JustLoaded())
			m_SelectedNodes.clear();

		ImGui::Begin("Scene Hierarchy");

		Ref<Scene> scene = SceneManager::Get().ActiveScene();
		SceneHierarchy& hierarchy = scene->GetHierarchy();

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
				scene->CreateEntity("Entity");
			if (ImGui::MenuItem("Create Abstract Entity"))
				scene->CreateAbstractEntity("Abstract Entity");
			if (m_CopiedNodes.size() > 0 && ImGui::MenuItem("Paste"))
			{
				UUID selectedNode = m_SelectedNodes.empty() ? UUID{ 0 } : m_SelectedNodes[0];
				for (auto& copiedNode : m_CopiedNodes)
					scene->CopyEntity(scene->GetEntity(copiedNode), selectedNode);
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		
		{
			if (m_SelectedNodes.size() > 0)
			{
				std::vector<Entity> selection;

				for (auto& node : m_SelectedNodes)
					selection.emplace_back(scene->GetEntity(node));

				DrawComponents(selection);
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				for (auto& componentKind : ComponentRegistry::GetAdditiveKinds())
				{
					if (ImGui::MenuItem(componentKind.Label().c_str()))
					{
						for (auto& id : m_SelectedNodes)
						{
							Entity entity = scene->GetEntity(id);
							if (!componentKind.Has(entity))
								componentKind.Add(entity);
						}

						ImGui::CloseCurrentPopup();
					}
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

	UUID SceneHierarchyPanel::GetSelectedEntity()
	{
		if (m_SelectedNodes.size() > 0)
			return m_SelectedNodes.at(0);

		return UUID{};
	}

	void SceneHierarchyPanel::DrawEntityNode(const EntityNode& node)
	{
		Ref<Scene> scene = SceneManager::Get().ActiveScene();
		SceneHierarchy& hierarchy = scene->GetHierarchy();

		Entity entity = scene->GetEntity(node.ID);
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
			scene->DestroyEntity(entity);
			m_SelectedNodes.clear();
		}
	}

	SceneHierarchyPanel::DrawComponentStatus TryDrawComponentSelection(std::vector<Entity>& entities, ComponentKind kind)
	{
		// return if not all entities have component T
		for (auto& entity : entities)
		{
			if (!kind.Has(entity))
				return SceneHierarchyPanel::DrawComponentStatus::None;
		}

		std::string componentLabel = kind.Label();
		bool open = ImGui::TreeNodeEx(componentLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap, componentLabel.c_str());
		bool removeComponent = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			kind.DrawSelection(entities);
			ImGui::TreePop();
		}

		if (removeComponent)
			return SceneHierarchyPanel::DrawComponentStatus::Removed;

		return SceneHierarchyPanel::DrawComponentStatus::None;
	}

	void SceneHierarchyPanel::DrawComponents(std::vector<Entity>& entitySelection)
	{
		using DrawComponentStatus = SceneHierarchyPanel::DrawComponentStatus;

		if (entitySelection.size() >= 1)
		{
			for (auto& componentKind : ComponentRegistry::GetBaseKinds())
				componentKind.DrawSelection(entitySelection);

			for (auto& componentKind : ComponentRegistry::GetAdditiveKinds())
			{
				DrawComponentStatus status = TryDrawComponentSelection(entitySelection, componentKind);

				if (status == DrawComponentStatus::Removed)
				{
					for (auto& entity : entitySelection)
						if (componentKind.Has(entity))
							componentKind.Remove(entity);
				}
			}
		}
	}
}