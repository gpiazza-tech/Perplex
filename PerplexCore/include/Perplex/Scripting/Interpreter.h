#pragma once

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Assets/Asset.h>

namespace Perplex
{
	class Scene;

	class Interpreter
	{
	public:
		void Start(Ref<Scene> scene);
		void Update(Ref<Scene> scene, Timestep ts);
		void Stop(Ref<Scene> scene);

		void OnScriptAssetReimported(Ref<Scene> scene, Asset asset);
	};
}