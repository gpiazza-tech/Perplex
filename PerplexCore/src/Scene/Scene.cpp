#include <Perplex/pch.h>
#include <Perplex/Scene/Scene.h>

#include <Perplex/Scene/Components.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/SceneHierarchy.h>
#include <Perplex/Core/UUID.h>

#include <entt.hpp>

#include <string>

namespace Perplex
{
	Entity Scene::CreateEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = Entity(m_Registry.create(), this);

		IDComponent& idc = entity.AddComponent<IDComponent>();
		idc.ID = uuid;
		m_UUIDMap[uuid] = (entt::entity)entity;

		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_Hierarchy.Add(EntityNode(entity.GetUUID()), parent);

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = Entity(m_Registry.create(), this);

		IDComponent& idc = entity.AddComponent<IDComponent>();
		idc.ID = uuid;
		m_UUIDMap[uuid] = (entt::entity)entity;

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_Hierarchy.Add(EntityNode(entity.GetUUID()), parent);

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Hierarchy.Remove(entity.GetUUID());

		m_UUIDMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::CopyEntity(Entity entity, UUID parent)
	{
		std::vector<UUID> children = m_Hierarchy.GetNode(entity.GetUUID()).ChildIDs;

		// copy main
		Entity newEntity = CreateAbstractEntity(entity.GetTag(), UUID(), parent);
		if (entity.HasComponent<TransformComponent>())
			newEntity.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
		if (entity.HasComponent<SpriteRendererComponent>())
			newEntity.AddComponent<SpriteRendererComponent>(entity.GetComponent<SpriteRendererComponent>());
		if (entity.HasComponent<CameraComponent>())
			newEntity.AddComponent<CameraComponent>(entity.GetComponent<CameraComponent>());
		if (entity.HasComponent<ScriptComponent>())
			newEntity.AddComponent<ScriptComponent>(entity.GetComponent<ScriptComponent>());

		// copy children
		for (auto& childID : children)
			CopyEntity(GetEntity(childID), newEntity.GetUUID());
	}

	Entity Scene::GetEntity(UUID uuid)
	{
		return Entity(m_UUIDMap[uuid], this);
	}
}