#include <pch.h>
#include "ScriptInstance.h"

#include "CUnit.h"
#include <Holloware/Core/Input.h>
#include <Holloware/Core/UUID.h>
#include <Holloware/Core/Log.h>
#include <Holloware/Core/Application.h>
#include <Holloware/Core/Project.h>
#include <Holloware/Scene/Scene.h>
#include <Holloware/Scene/Entity.h>
#include <Holloware/Scene/Components.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <string>

namespace Holloware
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
	static void try_call(Scene* scene, UUID uuid, const char* funcName)
	{
		Entity entity = scene->GetEntity(uuid);
		if (entity.HasComponent<ScriptComponent>())
			entity.GetComponent<ScriptComponent>().Instance.TryCall(funcName);
	}

	bool ScriptInstance::Compile(const std::string& src, Entity entity)
	{
		if (m_Unit.IsCompiled())
			return true;

		m_SceneContext = entity.GetScene();

		const Project& project = Application::Get().GetCurrentProject();
		m_Unit.AddIncludePath(project.EngineRes("scripting/include").string().c_str());

		m_Unit.DefineSymbol("PROPERTY", "__declspec(dllimport)");

		// Bind properties
		for (auto& property : entity.GetComponent<ScriptComponent>().Properties)
		{
			m_Unit.AddSymbol(property.GetName().c_str(), property.GetPtr());
		}

		// Bind transform
		m_Unit.AddSymbol("scene", &m_SceneContext);
		m_Unit.AddSymbol("entity", &entity.GetComponent<IDComponent>().ID);

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

		m_Unit.Compile(src.c_str());

		return m_Unit.IsCompiled();
	}
}