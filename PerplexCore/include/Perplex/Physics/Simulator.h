#pragma once

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

	class Simulator
	{
	public:
		void Start(Ref<Scene> scene);
		void Update(Ref<Scene> scene, Timestep ts);
		void Stop(Ref<Scene> scene);

		void SetVelocity(Entity entity, glm::vec2 velocity);
	private:
		int m_World{};
		std::unordered_map<UUID, uint64_t> m_Bodies = std::unordered_map<UUID, uint64_t>();
	};
}