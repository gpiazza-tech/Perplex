#include <pch.h>
#include "CUnit.h"

#include <Holloware/Core/Log.h>
#include <Holloware/Core/Application.h>
#include <Holloware/Core/Project.h>

#include "../../../vendor/tcc/libtcc.h"

#define STATE static_cast<TCCState*>(m_State)

namespace Holloware
{
	static constexpr int TCC_STATUS_FAIL = -1;

	CUnit::CUnit()
		: m_State(tcc_new()), m_IsCompiled(false)
	{
		tcc_set_error_func(STATE, nullptr, [](void* opaque, const char* msg) { HW_CORE_ERROR("C Script Error: {0}", msg); });

		const Project& project = Application::Get().GetCurrentProject();
		tcc_set_lib_path(STATE, project.EngineRes("scripting/tcc/lib").string().c_str());
		tcc_add_library_path(STATE, project.EngineRes("scripting/tcc/win32/lib").string().c_str());
		tcc_add_include_path(STATE, project.EngineRes("scripting/tcc/include").string().c_str());
		tcc_add_include_path(STATE, project.EngineRes("scripting/tcc/win32/include").string().c_str());

		tcc_set_output_type(STATE, TCC_OUTPUT_MEMORY);
	}

	CUnit::~CUnit()
	{
		tcc_delete(STATE);
	}

	bool CUnit::AddLibraryPath(const char* path)
	{
		return tcc_add_library_path(STATE, path) != TCC_STATUS_FAIL;
	}

	bool CUnit::AddLibrary(const char* library)
	{
		return tcc_add_library(STATE, library) != TCC_STATUS_FAIL;
	}

	bool CUnit::AddIncludePath(const char* path)
	{
		return tcc_add_include_path(STATE, path) != TCC_STATUS_FAIL;
	}

	bool CUnit::AddSymbol(const char* name, const void* value)
	{
		return tcc_add_symbol(STATE, name, value) != TCC_STATUS_FAIL;
	}

	void* CUnit::GetSymbol(const char* name)
	{
		return tcc_get_symbol(STATE, name);
	}

	bool CUnit::DefineSymbol(const char* name, const char* value)
	{
		tcc_define_symbol(STATE, name, value);
		return true;
	}

	bool CUnit::Compile(const char* string)
	{
		if (tcc_compile_string(STATE, string) == TCC_STATUS_FAIL)
		{
			m_IsCompiled = false;
			return false;
		}
		if (tcc_relocate(STATE) == TCC_STATUS_FAIL)
		{
			m_IsCompiled = false;
			return false;
		}

		m_IsCompiled = true;
		return true;
	}
}