#include <pch.h>
#include "Entity.h"

#include "Scene.h"
#include "Components.h"
#include <Holloware/Core/UUID.h>

#include <imgui.h>
#include <entt.hpp>
#include <string>

namespace Holloware
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	Entity::Entity(Scene* scene)
		: m_EntityHandle(scene->CreateEntity().m_EntityHandle), m_Scene(scene)
	{
	}

	UUID Entity::GetUUID() { return GetComponent<IDComponent>().ID; }
	std::string Entity::GetTag() { return GetComponent<TagComponent>().Tag; }

	bool Entity::operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	void Entity::DrawGui()
	{
		const std::string& label = (*this) ? GetComponent<TagComponent>().Tag : "NULL";
		ImGui::Button(label.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity* entity = (Entity*)payload->Data;
				*this = *entity;
			}
			ImGui::EndDragDropTarget();
		}
	}
}