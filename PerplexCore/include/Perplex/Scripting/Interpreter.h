#pragma once

#include <Perplex/Scripting/ScriptProperty.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Assets/Asset.h>

#include <vector>
#include <unordered_map>

namespace Perplex
{
	class Scene;
	class Simulator;
	class ScriptInstance;
	class Entity;

	class Interpreter
	{
	public:
		void Start(Ref<Scene> scene);
		void Update(Ref<Scene> scene, Timestep ts);
		void Stop(Ref<Scene> scene);

		void OnScriptAssetReimported(Ref<Scene> scene, Asset asset);
	private:
		void InitScriptInstance(Entity entity);
	private:
		std::unordered_map<UUID, ScriptInstance*> m_ScriptInstanceMap;
	};
}