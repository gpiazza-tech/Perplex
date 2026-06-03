#pragma once

#include <Perplex/Core/Core.h>

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
		SceneSystem(Ref<Scene> scene) : m_Scene(scene) {}
		virtual ~SceneSystem() = default;

		virtual void OnSceneStart();
		virtual void OnSceneUpdate(Timestep ts);
		virtual void OnSceneStop();

		virtual void OnEntityCreated(Entity entity);
		virtual void OnEntityDestroyed(Entity entity);

		virtual void OnComponentAdded(Component component, Entity entity);
		virtual void OnComponentRemoved(Component component, Entity entity);
	protected:
		Ref<Scene> m_Scene;
	};
}