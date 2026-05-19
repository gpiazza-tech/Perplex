#pragma once

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>

namespace Perplex
{
	class Scene;

	class Simulator
	{
	public:
		void Start(Ref<Scene> scene);
		void Update(Ref<Scene> scene, Timestep ts);
		void Stop(Ref<Scene> scene);
	};
}