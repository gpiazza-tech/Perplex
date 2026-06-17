#pragma once

#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Assets/Asset.h>

#include <unordered_map>

namespace Perplex
{
	class Scene;
	class Simulator;
	class ScriptInstance;
	class Entity;
	class Component;

	class Interpreter : SceneSystem
	{
	public:
		Interpreter(Ref<Scene> scene);

		TypeID GetComponentTypeID() override { return GetTypeID<ScriptComponent>(); }

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnComponentAdded(Entity entity) override;
		void OnComponentRemoved(Entity entity) override;

		void OnScriptAssetReimported(Ref<Scene> scene, Asset asset);

		ScriptInstance* GetInstance(UUID entityID);
	private:
		void InitScriptInstance(Entity entity);
	private:
		std::unordered_map<UUID, std::unique_ptr<ScriptInstance>> m_ScriptInstanceMap;
	};
}