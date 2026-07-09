#include <Perplex/pch.h>
#include <Perplex/Scene/Scene.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/SceneHierarchy.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Components/ComponentRegistry.h>
#include <Perplex/Components/ComponentKind.h>

#include <entt.hpp>

#include <string>
#include <vector>

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

		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		EnableComponent& enable = entity.AddComponent<EnableComponent>();

		m_Hierarchy.Add(EntityNode(entity.GetUUID()), parent);

		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = ConstructEntity(name, uuid, parent);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = ConstructEntity(name, uuid, parent);

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

		for (auto& componentKind : ComponentRegistry::GetAdditiveKinds())
			if (componentKind.Has(entity) && !componentKind.Has(newEntity))
				componentKind.Copy(entity, newEntity);

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
		m_Playing = true;

		for (SceneSystem* system : m_Systems)
			system->OnSceneStart();
	}

	void Scene::Update(Timestep ts)
	{
		ts *= m_Timescale;

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

				// Dispatch OnComponentRemoved callbacks
				for (SceneSystem* system : m_Systems)
					for (auto& componentKind : ComponentRegistry::GetAllKinds())
						if (componentKind.Has(entityToDestroy) && componentKind.GetTypeID() == system->GetComponentTypeID())
							system->OnComponentRemoved(entityToDestroy);

				// Actually destroy entity
				m_Hierarchy.Remove(entityToDestroy.GetUUID());

				m_UUIDMap.erase(entityToDestroy.GetUUID());
				m_Registry.destroy(entityToDestroy);
			}
		}
	}

	float Scene::GetTimescale() const
	{
		return m_Timescale;
	}

	void Scene::SetTimescale(float timescale)
	{
		m_Timescale = timescale;
	}

	void Scene::Stop()
	{
		m_Playing = false;

		for (SceneSystem* system : m_Systems)
			system->OnSceneStop();
	}
}