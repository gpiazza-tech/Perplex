#include <pch.h>
#include "ScriptInstance.h"

#include <Holloware/Core/Input.h>
#include <Holloware/Core/UUID.h>
#include <Holloware/Core/Log.h>
#include <Holloware/Core/Application.h>
#include <Holloware/Core/Project.h>
#include <Holloware/Scene/Scene.h>
#include <Holloware/Scene/Entity.h>
#include <Holloware/Scene/Components.h>

#include <libtcc.h>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <string>

namespace Holloware
{
	void* GetCSymbol(TCCState* state, const char* name) { return tcc_get_symbol(state, name); }

	ScriptInstance::~ScriptInstance()
	{
		if (m_State != nullptr)
		{
			tcc_delete(m_State);
			m_State = nullptr;
		}
	}

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
		Entity& entity = scene->GetEntity(uuid);
		if (entity.HasComponent<ScriptComponent>())
			entity.GetComponent<ScriptComponent>().Instance.TryCall(funcName);
	}

	bool ScriptInstance::Compile(const std::string& src, Entity entity)
	{
		m_SceneContext = entity.GetScene();

		if (m_State != nullptr)
		{
			tcc_delete(m_State);
			m_State = nullptr;
		}
		m_State = tcc_new();

		// Setup the script's environment.
		tcc_set_error_func(m_State, nullptr, [](void* opaque, const char* msg) { HW_CORE_ERROR("C Script Error: {0}", msg); });

		const Project& project = Application::Get().GetCurrentProject();

		// Include core tcc libs and scripts
		tcc_set_lib_path(m_State, project.EngineRes("scripting/tcc/lib").string().c_str());
		tcc_add_library_path(m_State, project.EngineRes("scripting/tcc/win32/lib").string().c_str());
		tcc_add_include_path(m_State, project.EngineRes("scripting/tcc/include").string().c_str());
		tcc_add_include_path(m_State, project.EngineRes("scripting/tcc/win32/include").string().c_str());

		// Include perplex scripts
		tcc_add_include_path(m_State, project.EngineRes("scripting/include").string().c_str());

		tcc_set_output_type(m_State, TCC_OUTPUT_MEMORY);

		tcc_define_symbol(m_State, "PROPERTY", "__declspec(dllimport)");

		// Bind properties
		for (auto& property : entity.GetComponent<ScriptComponent>().Properties)
		{
			tcc_add_symbol(m_State, property.GetName().c_str(), property.GetPtr());
		}

		// Bind transform
		tcc_add_symbol(m_State, "scene", &m_SceneContext);
		tcc_add_symbol(m_State, "entity", &entity.GetComponent<IDComponent>().ID);

		tcc_add_symbol(m_State, "position", &entity.GetComponent<TransformComponent>().Position);
		tcc_add_symbol(m_State, "rotation", &entity.GetComponent<TransformComponent>().Rotation);
		tcc_add_symbol(m_State, "scale", &entity.GetComponent<TransformComponent>().Scale);

		// Bind host functions
		tcc_add_symbol(m_State, "get_position_ptr", get_position_ptr);
		tcc_add_symbol(m_State, "get_rotation_ptr", get_rotation_ptr);
		tcc_add_symbol(m_State, "get_scale_ptr", get_scale_ptr);

		tcc_add_symbol(m_State, "console_trace", console_trace);
		tcc_add_symbol(m_State, "console_info", console_info);
		tcc_add_symbol(m_State, "console_warn", console_warn);
		tcc_add_symbol(m_State, "console_error", console_error);

		tcc_add_symbol(m_State, "key_pressed", Input::IsKeyPressed);
		tcc_add_symbol(m_State, "degrees", degrees);
		tcc_add_symbol(m_State, "radians", radians);

		tcc_add_symbol(m_State, "try_call", try_call);

		// Compile
		if (tcc_compile_string(m_State, src.c_str()) == -1)
		{
			tcc_delete(m_State);
			m_State = nullptr;
			return false;
		}

		if (tcc_relocate(m_State) == -1)
		{
			tcc_delete(m_State);
			m_State = nullptr;
			return false;
		}

		return true;
	}
}