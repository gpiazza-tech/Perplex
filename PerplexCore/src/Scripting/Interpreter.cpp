#include <Perplex/pch.h>
#include <Perplex/Scripting/Interpreter.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scripting/ScriptData.h>

namespace Perplex
{
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
			m_ScriptInstanceMap[entity.GetUUID()] = new ScriptInstance();
			ScriptInstance* instance = m_ScriptInstanceMap[entity.GetUUID()];
			instance->Compile(scriptData->Source, entity, sc.Properties);
		}
	}

	void Interpreter::Start(Ref<Scene> scene)
	{
		// Compile Scripts
		{
			auto view = scene->View<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity{ e, scene.get() };

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				InitScriptInstance(entity);
			}
		}

		// Call Start
		{
			auto view = scene->View<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity{ e, scene.get() };

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				ScriptInstance* instance = m_ScriptInstanceMap[entity.GetUUID()];
				instance->TryCall("start");
			}
		}
	}

	void Interpreter::Update(Ref<Scene> scene, Timestep ts)
	{
		// Call Update
		{
			auto view = scene->View<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity{ e, scene.get() };

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				if (!m_ScriptInstanceMap.contains(entity.GetUUID()))
				{
					InitScriptInstance(entity);
					m_ScriptInstanceMap[entity.GetUUID()]->TryCall("start");
				}

				ScriptInstance* instance = m_ScriptInstanceMap[entity.GetUUID()];
				instance->TryCall("update", ts.GetSeconds());
			}
		}
	}

	void Interpreter::Stop(Ref<Scene> scene)
	{
		// Call Stop
		{
			auto view = scene->View<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity{ e, scene.get() };

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				ScriptInstance* instance = m_ScriptInstanceMap[entity.GetUUID()];
				instance->TryCall("stop");
				delete instance;
				m_ScriptInstanceMap.erase(entity.GetUUID());
			}
		}
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
}