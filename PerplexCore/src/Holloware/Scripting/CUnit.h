#pragma once

namespace Holloware
{
	class CUnit
	{
	public:
		CUnit();
		~CUnit();

		bool AddLibraryPath(const char* path);
		bool AddLibrary(const char* library);
		bool AddIncludePath(const char* path);

		bool AddSymbol(const char* name, const void* value);
		void* GetSymbol(const char* name);
		bool DefineSymbol(const char* name, const char* value);

		bool Compile(const char* string);
		bool IsCompiled() const { return m_IsCompiled; };
	private:
		void* m_State;
		bool m_IsCompiled;
	};
}