#pragma once

#include <Perplex/Scene/SceneSystem.h>
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

	class Interpreter : SceneSystem
	{
	public:
		Interpreter(Ref<Scene> scene);

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnEntityCreated(UUID entityID) override;
		void OnEntityDestroyed(UUID entityID) override;

		void OnScriptAssetReimported(Ref<Scene> scene, Asset asset);

		ScriptInstance* GetInstance(UUID entityID);
	private:
		void InitScriptInstance(Entity entity);
	private:
		std::unordered_map<UUID, ScriptInstance*> m_ScriptInstanceMap;
	};
}