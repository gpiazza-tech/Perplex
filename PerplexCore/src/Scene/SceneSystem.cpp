#include <Perplex/pch.h>
#include <Perplex/Scene/SceneSerializer.h>

#include <Perplex/Core/Timestep.h>
#include <Perplex/Scene/Entity.h>

namespace Perplex
{
	void SceneSystem::OnSceneStart() {}
	void SceneSystem::OnSceneUpdate(Timestep ts) {}
	void SceneSystem::OnSceneStop() {}

	void SceneSystem::OnEntityCreated(Entity entity) {}
	void SceneSystem::OnEntityDestroyed(Entity entity) {}

	void SceneSystem::OnComponentAdded(const std::string& componentLabel, Entity entity) {}
	void SceneSystem::OnComponentRemoved(const std::string& componentLabel, Entity entity) {}
}