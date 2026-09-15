#pragma once

#include <Perplex/Angel/AngelEngine.h>
#include <Perplex/Scripting/ScriptProperty.h>

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

class asIScriptModule;
class asIScriptFunction;

namespace Perplex
{
	class AngelObject;
	class Entity;

	class AngelModule
	{
	public:
		template<typename ReturnType, typename... Args>
		ReturnType Call(asIScriptFunction* func, Args... args)
		{
			if (!func && std::is_void_v<ReturnType>)
				return;
			else if (!func && !std::is_void_v<ReturnType>)
				return ReturnType{};

			m_Engine->Prepare(func);
			 
			[&] <std::size_t... Is>(std::index_sequence<Is...>) {

				(m_Engine->SetArg(Is, args), ...);

			}(std::make_index_sequence<sizeof...(Args)>());

			m_Engine->Execute();

			if constexpr (!std::is_void_v<ReturnType>)
				return m_Engine->GetReturn<ReturnType>();
		}

		template<typename ReturnType, typename... Args>
		ReturnType CallMethod(void* obj, asIScriptFunction* func, Args... args)
		{
			if (!func && std::is_void_v<ReturnType>)
				return;
			else if (!func && !std::is_void_v<ReturnType>)
				return ReturnType{};

			m_Engine->Prepare(func);
			m_Engine->SetObject(obj);

			[&] <std::size_t... Is>(std::index_sequence<Is...>) {

				(m_Engine->SetArg(Is, args), ...);

			}(std::make_index_sequence<sizeof...(Args)>());

			m_Engine->Execute();

			if constexpr (!std::is_void_v<ReturnType>)
				return m_Engine->GetReturn<ReturnType>();
		}

		AngelObject CreateObject(Entity entity);
		AngelObject CreateObject(const std::string& className, Entity entity);

		asIScriptFunction* GetMethod(const std::string& className, const std::string& funcName);

		AngelEngine& Engine() { return *m_Engine; }
		const std::string& Name() { return m_Name; }
		const std::vector<ScriptProperty>& Properties() { return m_Properties; }
	private:
		AngelModule(asIScriptModule* mod, AngelEngine* ctx, const std::string& name);

		asIScriptFunction* GetFunctionByName(const std::string& funcName);
	private:
		asIScriptModule* m_Module;
		AngelEngine* m_Engine;
		std::string m_Name;
		std::vector<ScriptProperty> m_Properties;

		friend class AngelEngine;
	};
}