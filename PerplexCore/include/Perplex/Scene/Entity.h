#pragma once

#include "Components.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Components/Component.h>
#include <Perplex/Components/ComponentLabelers.h>
#include <Perplex/Components/ComponentDrawers.h>

#include <glm/fwd.hpp>

#include <entt.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>

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
			HW_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			entt::entity handle = m_EntityHandle;
			Scene* scene = m_Scene;

			// Remove polymorphic component
			std::vector<Component>& components = scene->m_ComponentsMap[handle];
			std::erase(components, Component(T{}));

			// Remove actual component
			scene->m_Registry.remove<T>(handle);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&& ...args)
		{
			HW_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

			// Add actual component
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

			// Add polymorphic component
			auto componentGetter = [*this]() mutable -> T& { return GetComponent<T>(); };
			auto componentRemover = [*this]() mutable -> void { RemoveComponent<T>(); };
			m_Scene->m_ComponentsMap[m_EntityHandle].push_back(Component(component, componentGetter, componentRemover));
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			HW_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		std::vector<Component>& GetComponents()
		{
			return m_Scene->m_ComponentsMap[m_EntityHandle];
		}

		const std::vector<Component>& GetComponents() const
		{
			return m_Scene->m_ComponentsMap[m_EntityHandle];
		}

		TransformComponent GetGlobalTransform();

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