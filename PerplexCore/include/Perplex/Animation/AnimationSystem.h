#pragma once

#include <Perplex/Animation/AnimatorInstance.h>
#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/Scene.h>

#include <unordered_map>

namespace Perplex
{
	class AnimationSystem : SceneSystem
	{
	public:
		AnimationSystem(Ref<Scene> scene) : SceneSystem{ scene }, m_AnimatorMap{} {}
		virtual ~AnimationSystem() = default;

		TypeID GetComponentTypeID() override { return GetTypeID<SpriteAnimatorComponent>(); };

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnComponentAdded(Entity entity) override;
		void OnComponentRemoved(Entity entity) override;
	private:
		std::unordered_map<UUID, AnimatorInstance> m_AnimatorMap;
	};
}