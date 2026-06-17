#pragma once

#include <Perplex/Core/Core.h>
#include <Perplex/Core/TypeID.h>

namespace Perplex
{
	class UUID;
	class Timestep;
	class Scene;
	class Entity;
	class Component;

	class SceneSystem
	{
	public:
		SceneSystem(Ref<Scene> scene) : m_Scene{ scene } {}
		virtual ~SceneSystem() = default;

		virtual TypeID GetComponentTypeID() = 0;

		virtual void OnSceneStart() = 0;
		virtual void OnSceneUpdate(Timestep ts) = 0;
		virtual void OnSceneStop() = 0;

		virtual void OnComponentAdded(Entity entity) = 0;
		virtual void OnComponentRemoved(Entity entity) = 0;
	protected:
		Ref<Scene> m_Scene;
	};
}