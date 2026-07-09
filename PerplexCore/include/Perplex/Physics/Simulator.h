#pragma once

#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/PTL/bimap.h>

#include <glm/fwd.hpp>

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
		~Simulator();

		TypeID GetComponentTypeID() override { return GetTypeID<BoxColliderComponent>(); }

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnComponentAdded(Entity entity) override;
		void OnComponentRemoved(Entity entity) override;

		void SetVelocity(UUID entityID, glm::vec2 velocity);

		float GetTimescale() const;
		void SetTimescale(float timescale);
	private:
		void AddCollider(Entity entity);
	private:
		int m_World{};
		ptl::bimap<UUID, uint64_t> m_BodyMap{};
		float m_Timescale{ 1.0f };
	};
}