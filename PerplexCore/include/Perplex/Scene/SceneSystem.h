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

		virtual void OnSceneStart();
		virtual void OnSceneUpdate(Timestep ts);
		virtual void OnSceneStop();

		virtual void OnComponentAdded(Entity entity);
		virtual void OnComponentRemoved(Entity entity);
	protected:
		Ref<Scene> m_Scene;
	};
}