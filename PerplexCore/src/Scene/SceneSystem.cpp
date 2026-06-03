#include <Perplex/pch.h>
#include <Perplex/Scene/SceneSerializer.h>

#include <Perplex/Core/Timestep.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Components/Component.h>

namespace Perplex
{
	void SceneSystem::OnSceneStart() {}
	void SceneSystem::OnSceneUpdate(Timestep ts) {}
	void SceneSystem::OnSceneStop() {}

	void SceneSystem::OnEntityCreated(Entity entity) {}
	void SceneSystem::OnEntityDestroyed(Entity entity) {}

	void SceneSystem::OnComponentAdded(Component component, Entity entity) {}
	void SceneSystem::OnComponentRemoved(Component component, Entity entity) {}
}