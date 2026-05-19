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

				Ref<ScriptData> scriptData = sc.ScriptAsset.GetData<ScriptData>();
				if (scriptData) sc.Instance.Compile(scriptData->Source, entity);
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

				sc.Instance.TryCall("start");
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

				sc.Instance.TryCall("update", ts.GetSeconds());
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

				sc.Instance.TryCall("stop");
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