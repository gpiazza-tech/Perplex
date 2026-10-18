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

#include <memory>
#include <vector>
#include <Perplex/Angel/AngelModule.h>
#include <optional>

namespace Perplex
{
	Interpreter::Interpreter(Ref<Scene> scene) : SceneSystem(scene) {}

	void Interpreter::OnSceneStart()
	{
		/*
		// Compile Scripts
		auto view = m_Scene->View<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };

			auto& sc = view.get<ScriptComponent>(e);
			TagComponent& tag = entity.GetComponent<TagComponent>();

			InitScriptInstance(entity);
		}
		*/
	}

	void Interpreter::OnSceneUpdate(Timestep ts)
	{
		HW_PROFILE_FUNCTION();

		// Call Update
		auto view = m_Scene->View<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };
			auto& sc = view.get<ScriptComponent>(e);

			if (sc.AlwaysReceiveUpdates || !m_Scene->IsPaused())
			{
				UUID entityID = entity.GetUUID();

				if (!m_ScriptInstanceMap.contains(entityID))
				{
					Ref<AngelModule> angelModule = sc.ScriptAsset.GetData<AngelModule>();
					if (angelModule)
						m_ScriptInstanceMap.emplace(entityID, angelModule->CreateObject(entity, sc.Properties));
					m_ScriptInstanceMap.at(entityID).Call<void>("Start");
				}
				m_ScriptInstanceMap.at(entityID).Call<void>("Update", ts.GetSeconds());
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

			m_ScriptInstanceMap.at(entityID).Call<void>("Stop");
			m_ScriptInstanceMap.erase(entityID);
		}
	}

	void Interpreter::OnComponentAdded(Entity entity)
	{
	}

	void Interpreter::OnComponentRemoved(Entity entity)
	{
		UUID entityID = entity.GetUUID();

		if (m_ScriptInstanceMap.contains(entityID))
		{
			m_ScriptInstanceMap.at(entityID).Release();
			m_ScriptInstanceMap.erase(entityID);
		}
	}

	void Interpreter::OnScriptAssetReimported(Asset asset)
	{
		auto view = m_Scene->View<ScriptComponent>();
		for (auto e : view)
		{
			auto& sc = view.get<ScriptComponent>(e);

			if ((UUID)sc.ScriptAsset == (UUID)asset)
			{
				// SyncProperties(sc);
				HW_CORE_WARN("Property syncing not implemented!");
			}
		}
	}

	std::optional<AngelObject> Interpreter::GetInstance(UUID entityID)
	{
		if (m_ScriptInstanceMap.contains(entityID))
			return m_ScriptInstanceMap.at(entityID);
		return std::nullopt;
	}

	void Interpreter::InvokeEvent(const char* eventName, void* data)
	{
		for (auto& scriptInstance : m_ScriptInstanceMap)
			scriptInstance.second.Call<void>(eventName, data);
	}
}