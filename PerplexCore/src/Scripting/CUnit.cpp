#include <Perplex/pch.h>
#include <Perplex/Scripting/CUnit.h>

#include <Perplex/Core/Log.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Project.h>

#include "../../../vendor/tcc/libtcc.h"

namespace Perplex
{
	static constexpr int TCC_STATUS_FAIL = -1;

	CUnit::CUnit()
		: m_State(tcc_new()), m_IsCompiled(false)
	{
		tcc_set_error_func(m_State, nullptr, [](void* opaque, const char* msg) { HW_CORE_ERROR("C Script Error: {0}", msg); });

		const Project& project = Application::Get().GetCurrentProject();
		tcc_set_lib_path(m_State, project.EngineRes("scripting/tcc/lib").string().c_str());
		tcc_add_library_path(m_State, project.EngineRes("scripting/tcc/win32/lib").string().c_str());
		tcc_add_include_path(m_State, project.EngineRes("scripting/tcc/include").string().c_str());
		tcc_add_include_path(m_State, project.EngineRes("scripting/tcc/win32/include").string().c_str());

		tcc_set_output_type(m_State, TCC_OUTPUT_MEMORY);
	}

	CUnit::CUnit(const CUnit& other)
		: m_State(tcc_new()), m_IsCompiled(false)
	{
		tcc_set_error_func(m_State, nullptr, [](void* opaque, const char* msg) { HW_CORE_ERROR("C Script Error: {0}", msg); });

		const Project& project = Application::Get().GetCurrentProject();
		tcc_set_lib_path(m_State, project.EngineRes("scripting/tcc/lib").string().c_str());
		tcc_add_library_path(m_State, project.EngineRes("scripting/tcc/win32/lib").string().c_str());
		tcc_add_include_path(m_State, project.EngineRes("scripting/tcc/include").string().c_str());
		tcc_add_include_path(m_State, project.EngineRes("scripting/tcc/win32/include").string().c_str());

		tcc_set_output_type(m_State, TCC_OUTPUT_MEMORY);
	}

	CUnit::~CUnit()
	{
		tcc_delete(m_State);
	}

	bool CUnit::AddLibraryPath(const char* path)
	{
		return tcc_add_library_path(m_State, path) != TCC_STATUS_FAIL;
	}

	bool CUnit::AddLibrary(const char* library)
	{
		return tcc_add_library(m_State, library) != TCC_STATUS_FAIL;
	}

	bool CUnit::AddIncludePath(const char* path)
	{
		return tcc_add_include_path(m_State, path) != TCC_STATUS_FAIL;
	}

	bool CUnit::AddSymbol(const char* name, const void* value)
	{
		return tcc_add_symbol(m_State, name, value) != TCC_STATUS_FAIL;
	}

	void* CUnit::GetSymbol(const char* name)
	{
		return tcc_get_symbol(m_State, name);
	}

	bool CUnit::DefineSymbol(const char* name, const char* value)
	{
		tcc_define_symbol(m_State, name, value);
		return true;
	}

	bool CUnit::Compile(const char* string)
	{
		if (tcc_compile_string(m_State, string) == TCC_STATUS_FAIL)
		{
			m_IsCompiled = false;
			return false;
		}
		if (tcc_relocate(m_State) == TCC_STATUS_FAIL)
		{
			m_IsCompiled = false;
			return false;
		}

		m_IsCompiled = true;
		return true;
	}
}