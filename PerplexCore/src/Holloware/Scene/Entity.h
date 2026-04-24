#pragma once

#include <Holloware/Core/Core.h>
#include <Holloware/Core/Log.h>
#include <Holloware/Core/HollowareObject.h>
#include <Holloware/Scene/Scene.h>

#include <entt.hpp>
#include <cstdint>
#include <string>

namespace Holloware
{
	class UUID;

	class Entity : public HollowareObject
	{
	public:
		Entity() = default;
		Entity(Scene* scene);
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename ...Args>
		T& AddComponent(Args && ...args)
		{
			HW_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
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

		template<typename T>
		void RemoveComponent()
		{
			HW_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		UUID GetUUID();
		std::string GetTag();
		Scene* GetScene() { return m_Scene; };

		void DrawGui() override;

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