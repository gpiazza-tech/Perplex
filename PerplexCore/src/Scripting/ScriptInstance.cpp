#include <Perplex/pch.h>
#include <Perplex/Scripting/ScriptInstance.h>

#include <Perplex/Scripting/CUnit.h>
#include <Perplex/Core/Input.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Project.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Physics/Simulator.h>
#include <Perplex/Audio/AudioEngine.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <string>

namespace Perplex
{
	static glm::vec3* get_position_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<TransformComponent>().Position; }
	static glm::vec3* get_rotation_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<TransformComponent>().Rotation; }
	static glm::vec3* get_scale_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<TransformComponent>().Scale; }
	static void console_trace(const char* msg) { HW_TRACE(msg); }
	static void console_info(const char* msg) { HW_INFO(msg); }
	static void console_warn(const char* msg) { HW_WARN(msg); }
	static void console_error(const char* msg) { HW_ERROR(msg); }
	static float degrees(float rad) { return glm::degrees(rad); }
	static float radians(float deg) { return glm::radians(deg); }
	static void play_sound(const char* filepath) { AudioEngine::Get().PlaySound(filepath); }

	static void try_call(Scene* scene, UUID uuid, const char* funcName)
	{
		ScriptInstance* instance = scene->GetSystem<Interpreter>().GetInstance(uuid);

		if (instance != nullptr)
		{
			instance->TryCall(funcName);
		}
		else
		{
			HW_CORE_WARN("Tried to call function on entity with no ScriptComponent!");
		}
	}

	static long long _spawn(Scene* scene, uint64_t prefabAssetID)
	{
		Asset prefabAsset{ UUID{ prefabAssetID} };
		if (!prefabAsset)
		{
			HW_ERROR("Trying to spawn empty prefab!");
			return static_cast<long long>(UUID{});
		}

		Ref<Scene> prefabScene = prefabAsset.GetData<Scene>();
 		Entity newEntity = scene->CopyEntity(prefabScene->GetParentEntities().at(0));
		UUID newEntityID = newEntity.GetUUID();
		return static_cast<long long>(newEntityID);
	}

	static void _destroy(Scene* scene, UUID entity)
	{
		scene->DestroyEntity(scene->GetEntity(entity));
	}

	static void _destroy_delay(Scene* scene, UUID entity, float delay)
	{
		scene->DestroyEntityDelay(scene->GetEntity(entity), delay);
	}

	static void _set_velocity(Scene* scene, UUID entity, glm::vec2 velocity)
	{
		scene->GetSystem<Simulator>().SetVelocity(entity, velocity);
	}

	bool ScriptInstance::Compile(const std::string& src, Entity entity, const std::vector<ScriptProperty>& properties)
	{
		if (m_Unit.IsCompiled())
			return true;
		
		m_EntityID = entity.GetUUID();;
		m_Properties = properties;
		m_SceneContext = entity.GetScene();

		const Project& project = Application::Get().GetCurrentProject();
		m_Unit.AddIncludePath(project.EngineRes("scripting/include/c").string().c_str());

		m_Unit.DefineSymbol("PROPERTY", "__declspec(dllimport)");

		// Bind properties
		for (auto& property : m_Properties)
		{
			m_Unit.AddSymbol(property.GetName().c_str(), property.GetPtr());
		}

		// Bind transform
		m_Unit.AddSymbol("scene", &m_SceneContext);
		m_Unit.AddSymbol("this", &m_EntityID);

		// Bind host functions
		m_Unit.AddSymbol("get_position_ptr", get_position_ptr);
		m_Unit.AddSymbol("get_rotation_ptr", get_rotation_ptr);
		m_Unit.AddSymbol("get_scale_ptr", get_scale_ptr);

		m_Unit.AddSymbol("console_trace", console_trace);
		m_Unit.AddSymbol("console_info", console_info);
		m_Unit.AddSymbol("console_warn", console_warn);
		m_Unit.AddSymbol("console_error", console_error);

		m_Unit.AddSymbol("key_pressed", Input::IsKeyPressed);
		m_Unit.AddSymbol("degrees", degrees);
		m_Unit.AddSymbol("radians", radians);

		m_Unit.AddSymbol("try_call", try_call);

		m_Unit.AddSymbol("_spawn", _spawn);
		m_Unit.AddSymbol("_destroy", _destroy);
		m_Unit.AddSymbol("_destroy_delay", _destroy_delay);
		m_Unit.AddSymbol("_set_velocity", _set_velocity);

		m_Unit.AddSymbol("play_sound", play_sound);

		for (auto& externalFunctions : m_ExternalFunctions)
			m_Unit.AddSymbol(externalFunctions.Name.c_str(), externalFunctions.Ptr);

		m_Unit.Compile(src.c_str());

		return m_Unit.IsCompiled();
	}

	void ScriptInstance::AddFunction(const ExternalFunction& function)
	{
		m_ExternalFunctions.emplace_back(function);
	}
}