#include <Perplex/pch.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Scene/SceneSystem.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scripting/ScriptData.h>
#include <Perplex/Scripting/ScriptInstance.h>
#include <Perplex/Scripting/ScriptProperty.h>
#include <Perplex/Perpixel/PerpixelSystem.h>

#include <memory>
#include <vector>

namespace Perplex
{
	Interpreter::Interpreter(Ref<Scene> scene) : SceneSystem(scene) {}

	static void SyncProperties(ScriptComponent& scriptComponent)
	{
		std::vector<ScriptProperty> oldProperties = scriptComponent.Properties;
		scriptComponent.Properties = scriptComponent.ScriptAsset.GetData<ScriptData>()->Properties;

		for (auto& oldProperty : oldProperties)
		{
			for (auto& newProperty : scriptComponent.Properties)
			{
				newProperty.TrySync(oldProperty);
			}
		}
	}

	void Interpreter::InitScriptInstance(Entity entity)
	{
		ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
		Ref<ScriptData> scriptData = sc.ScriptAsset.GetData<ScriptData>();
		if (scriptData)
		{
			m_ScriptInstanceMap[entity.GetUUID()] = std::make_unique<ScriptInstance>();
			UUID entityID = entity.GetUUID();

			std::unique_ptr<ScriptInstance>& instance = m_ScriptInstanceMap[entity.GetUUID()];
			instance->Compile(scriptData->SourceName, scriptData->Source, entity, sc.Properties);
		}
	}

	void Interpreter::OnSceneStart()
	{
		// Compile Scripts
		auto view = m_Scene->View<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };

			auto& sc = view.get<ScriptComponent>(e);
			TagComponent& tag = entity.GetComponent<TagComponent>();

			InitScriptInstance(entity);
		}
	}

	void Interpreter::OnSceneUpdate(Timestep ts)
	{
		// Call Update
		auto view = m_Scene->View<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };
			auto& sc = view.get<ScriptComponent>(e);

			if (sc.AlwaysReceiveUpdates || !m_Scene->IsPaused())
			{
				UUID entityID = entity.GetUUID();

				std::unique_ptr<ScriptInstance>& instance = m_ScriptInstanceMap.at(entityID);

				if (!instance->Started())
				{
					instance->SetStarted(true);
					instance->TryCall("start");
				}
				instance->TryCall("update", ts.GetSeconds());
			}
		}
	}

	void Interpreter::OnSceneStop()
	{
		// Call Stop
		auto view = m_Scene->View<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };

			auto& sc = view.get<ScriptComponent>(e);
			UUID entityID = entity.GetUUID();

			std::unique_ptr<ScriptInstance>& instance = m_ScriptInstanceMap[entityID];
			instance->TryCall("stop");
			m_ScriptInstanceMap.erase(entityID);
		}
	}

	void Interpreter::OnComponentAdded(Entity entity)
	{
		if (m_Scene->IsPlaying() && entity.HasComponent<ScriptComponent>())
			InitScriptInstance(entity);
	}

	void Interpreter::OnComponentRemoved(Entity entity)
	{
		UUID entityID = entity.GetUUID();

		if (m_ScriptInstanceMap.contains(entityID))
			m_ScriptInstanceMap.erase(entityID);
	}

	void Interpreter::OnScriptAssetReimported(Ref<Scene> scene, Asset asset)
	{
		auto view = scene->View<ScriptComponent>();
		for (auto e : view)
		{
			auto& sc = view.get<ScriptComponent>(e);

			if ((UUID)sc.ScriptAsset == (UUID)asset)
			{
				SyncProperties(sc);
			}
		}
	}

	ScriptInstance* Interpreter::GetInstance(UUID entityID)
	{
		// TODO: returning unique_ptr::get, a safer alternative should be implemented

		if (m_ScriptInstanceMap.contains(entityID))
			return m_ScriptInstanceMap[entityID].get();
		return nullptr;
	}
}