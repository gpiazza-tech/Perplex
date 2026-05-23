#pragma once

#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>

#include <glm/fwd.hpp>

#include <unordered_map>
#include <cstdint>

namespace Perplex
{
	class Scene;
	class Entity;
	
	void HitEnter(Ref<Scene> scene, UUID first, UUID second);
	void HitExit(Ref<Scene> scene, UUID first, UUID second);

	class Simulator : SceneSystem
	{
	public:
		Simulator(Ref<Scene> scene);

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnEntityCreated(UUID entityID) override;
		void OnEntityDestroyed(UUID entityID) override;

		void SetVelocity(UUID entityID, glm::vec2 velocity);
	private:
		void AddCollider(Entity entity);
	private:
		int m_World{};
		std::unordered_map<UUID, uint64_t> m_BodyMap = std::unordered_map<UUID, uint64_t>();
		std::unordered_map<uint64_t, UUID> m_UUIDMap = std::unordered_map<uint64_t, UUID>();
	};
}