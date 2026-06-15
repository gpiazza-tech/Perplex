#pragma once

#include "Components.h"
#include "SceneSystem.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Components/ComponentLabelers.h>

#include <entt.hpp>
#include <cstdint>
#include <string>
#include <utility>

namespace Perplex 
{
	class UUID;

	class Entity
	{
	public:
		Entity() = default;
		Entity(Scene* scene);
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		void RemoveComponent()
		{
			HW_CORE_ASSERT(HasComponent<T>(), "Tried to remove non-existent component type '{0}' on entity '{1}'", Label((T&)(T{})).c_str(), GetTag().c_str());

			entt::entity handle = m_EntityHandle;
			Scene* scene = m_Scene;

			// Remove actual component
			scene->m_Registry.remove<T>(handle);

			// OnComponentRemoved callbacks
			for (SceneSystem* system : scene->m_Systems)
				if (system->GetComponentTypeID() == GetTypeID<T>())
					system->OnComponentRemoved(*this);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&& ...args)
		{
			HW_CORE_ASSERT(!HasComponent<T>(), "Entity '{0}' already contains component type '{1}'", GetTag().c_str(), Label((T&)(T{})).c_str());

			// Add actual component
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

			// OnComponentAdded callbacks
			for (SceneSystem* system : m_Scene->m_Systems)
				if (system->GetComponentTypeID() == GetTypeID<T>())
					system->OnComponentAdded(*this);

			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			HW_CORE_ASSERT(HasComponent<T>(), "Tried to get non-existent component type '{0}' on entity '{1}'", Label((T&)(T{})).c_str(), GetTag().c_str());
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		TransformComponent GetGlobalTransform();
		void SetGlobalTransform(const TransformComponent& transform);

		UUID GetUUID();
		std::string GetTag();
		Scene* GetScene() { return m_Scene; };

		void DrawGui();

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const{ return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const;
		bool operator!=(const Entity& other) const;
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}