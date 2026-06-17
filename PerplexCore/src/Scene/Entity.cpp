#include <Perplex/pch.h>
#include <Perplex/Scene/Entity.h>

#include <Perplex/Scene/EntityNode.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/SceneHierarchy.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/UUID.h>

#include <imgui.h>
#include <entt.hpp>

#include <string>

namespace Perplex
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
	std::string Entity::GetTag() { return HasComponent<TagComponent>() ? GetComponent<TagComponent>().Tag : "NULL_TAG"; }

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

	TransformComponent Entity::GetGlobalTransform()
	{
		TransformComponent transform = GetComponent<TransformComponent>();
		const EntityNode& node = m_Scene->GetHierarchy().GetNode(GetUUID());

		if (node.ParentID == 0)
			return transform;

		const TransformComponent& parentTransform = m_Scene->GetEntity(node.ParentID).GetGlobalTransform();
		/*
		glm::mat4 newTransform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

		// 2. Rotate (applied second)
		model = glm::rotate(model, glm::radians(angle), glm::vec3(axis_x, axis_y, axis_z));

		// 3. Scale (applied first)
		model = glm::scale(model, glm::vec3(sx, sy, sz));

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(transform, scale, rotation, translation, skew, perspective);
		*/
		
		transform.Position += parentTransform.Position;
		transform.Rotation += parentTransform.Rotation;
		transform.Scale *= parentTransform.Scale;

		return transform;
	}

	void Entity::SetGlobalTransform(const TransformComponent& transform)
	{
		EntityNode node = m_Scene->GetHierarchy().GetNode(GetUUID());
		TransformComponent newTransform{ transform };

		while (node.ParentID != 0)
		{
			const TransformComponent& parentTransform = m_Scene->GetEntity(node.ParentID).GetComponent<TransformComponent>();

			newTransform.Position -= parentTransform.Position;
			newTransform.Rotation -= parentTransform.Rotation;
			newTransform.Scale /= parentTransform.Scale;

			node = m_Scene->GetHierarchy().GetNode(node.ParentID);
		}

		m_Scene->GetEntity(node.ID).GetComponent<TransformComponent>() = newTransform;
	}
}