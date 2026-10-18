#pragma once

#include <Perplex/Angel/AngelModule.h>

#include <type_traits>

class asIScriptObject;
class asIScriptFunction;

namespace Perplex
{
	class AngelEngine;

	class AngelObject
	{
	public:
		template<typename ReturnType, typename... Args>
		ReturnType Call(const char* funcName, Args... args)
		{
			if (!m_Object)
			{
				if constexpr (std::is_void_v<ReturnType>)
					return;
				else
					return ReturnType{};
			}

			asIScriptFunction* func = m_Module->GetMethod(m_Module->Name(), funcName);

			if constexpr (std::is_void_v<ReturnType>)
				m_Module->CallMethod<ReturnType>(m_Object, func, args...);
			else
				return m_Module->CallMethod<ReturnType>(m_Object, func, args...);
		}

		void Release();
	private:
		AngelObject(asIScriptObject* obj, AngelModule* mod);
	private:
		asIScriptObject* m_Object;
		AngelModule* m_Module;

		friend AngelModule;
	};
}