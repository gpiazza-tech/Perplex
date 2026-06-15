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

	void SceneSystem::OnComponentAdded(Entity entity) {}
	void SceneSystem::OnComponentRemoved(Entity entity) {}
}