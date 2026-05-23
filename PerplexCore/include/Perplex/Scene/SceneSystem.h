#pragma once

#include <Perplex/Core/Core.h>

namespace Perplex
{
	class UUID;
	class Timestep;
	class Scene;

	class SceneSystem
	{
	public:
		SceneSystem(Ref<Scene> scene) : m_Scene(scene) {}
		virtual ~SceneSystem() = default;

		virtual void OnSceneStart() = 0;
		virtual void OnSceneUpdate(Timestep ts) = 0;
		virtual void OnSceneStop() = 0;

		virtual void OnEntityCreated(UUID createdEntityID) = 0;
		virtual void OnEntityDestroyed(UUID destroyedEntityID) = 0;
	protected:
		Ref<Scene> m_Scene;
	};
}