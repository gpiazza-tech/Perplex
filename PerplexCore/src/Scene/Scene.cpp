#include <Perplex/pch.h>
#include <Perplex/Scene/Scene.h>

#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/SceneHierarchy.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Timestep.h>

#include <entt.hpp>

#include <string>

namespace Perplex
{
	Scene::Scene()
	{
		m_SystemsContainer = m_Registry.create();
	}

	Entity Scene::ConstructEntity(const std::string& name, UUID uuid, UUID parent)
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

	Entity Scene::CreateEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = ConstructEntity(name, uuid, parent);

		entity.AddComponent<TransformComponent>();

		for (SceneSystem* system : m_Systems)
			system->OnEntityCreated(uuid);

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = ConstructEntity(name, uuid, parent);

		for (SceneSystem* system : m_Systems)
			system->OnEntityCreated(uuid);

		return entity;
	}

	void Scene::DestroyEntity(Entity entity, float delay)
	{
		m_DyingEntities.emplace_back(entity.GetUUID(), delay);
	}

	Entity Scene::CopyEntity(Entity entity, UUID parent)
	{
		std::vector<UUID> children = m_Hierarchy.GetNode(entity.GetUUID()).ChildIDs;

		// copy main
		Entity newEntity = ConstructEntity(entity.GetTag(), UUID(), parent);
		if (entity.HasComponent<TransformComponent>())
			newEntity.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
		if (entity.HasComponent<SpriteRendererComponent>())
			newEntity.AddComponent<SpriteRendererComponent>(entity.GetComponent<SpriteRendererComponent>());
		if (entity.HasComponent<CameraComponent>())
			newEntity.AddComponent<CameraComponent>(entity.GetComponent<CameraComponent>());
		if (entity.HasComponent<ScriptComponent>())
			newEntity.AddComponent<ScriptComponent>(entity.GetComponent<ScriptComponent>());
		if (entity.HasComponent<BoxColliderComponent>())
			newEntity.AddComponent<BoxColliderComponent>(entity.GetComponent<BoxColliderComponent>());
		if (entity.HasComponent<PhysicsBodyComponent>())
			newEntity.AddComponent<PhysicsBodyComponent>(entity.GetComponent<PhysicsBodyComponent>());

		for (SceneSystem* system : m_Systems)
			system->OnEntityCreated(newEntity.GetUUID());

		// copy children
		for (auto& childID : children)
			CopyEntity(GetEntity(childID), newEntity.GetUUID());

		return newEntity;
	}

	Entity Scene::GetEntity(UUID uuid)
	{
		return Entity(m_UUIDMap[uuid], this);
	}

	std::vector<Entity> Scene::GetParentEntities()
	{
		std::vector<Entity> entities{};

		for (auto& rootUUID : m_Hierarchy.GetRoot().ChildIDs)
			entities.emplace_back(m_UUIDMap[rootUUID], this);

		return entities;
	}

	void Scene::Start()
	{
		for (SceneSystem* system : m_Systems)
			system->OnSceneStart();
	}

	void Scene::Update(Timestep ts)
	{
		for (SceneSystem* system : m_Systems)
			system->OnSceneUpdate(ts);

		for (size_t i{}; i < m_DyingEntities.size(); ++i)
		{
			m_DyingEntities[i].Time -= ts;

			if (m_DyingEntities[i].Time <= 0.0f)
			{
				Entity entityToDestroy = GetEntity(m_DyingEntities[i].EntityID);
				m_DyingEntities.erase(m_DyingEntities.begin() + i);
				--i;

				// Check if entity has ID. This can be false if the entity is
				// being destroyed on the same frame that it was created
				if (!entityToDestroy.HasComponent<IDComponent>())
					continue;

				// Destroy callback
				for (SceneSystem* system : m_Systems)
					system->OnEntityDestroyed(entityToDestroy.GetUUID());

				// Actually destroy entity
				m_Hierarchy.Remove(entityToDestroy.GetUUID());

				m_ComponentsMap.erase(entityToDestroy);
				m_UUIDMap.erase(entityToDestroy.GetUUID());
				m_Registry.destroy(entityToDestroy);
			}
		}
	}

	void Scene::Stop()
	{
		for (SceneSystem* system : m_Systems)
			system->OnSceneStop();
	}
}