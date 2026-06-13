#include <Perplex/pch.h>

#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Scene/Entity.h>

#include <string>

namespace Perplex
{
	void SceneSystem::OnSceneStart() {}
	void SceneSystem::OnSceneUpdate(Timestep ts) {}
	void SceneSystem::OnSceneStop() {}

	void SceneSystem::OnEntityCreated(Entity entity) {}
	void SceneSystem::OnEntityDestroyed(Entity entity) {}

	void SceneSystem::OnComponentAdded(TypeID componentTypeID, Entity entity) {}
	void SceneSystem::OnComponentRemoved(TypeID componentTypeID, Entity entity) {}
}