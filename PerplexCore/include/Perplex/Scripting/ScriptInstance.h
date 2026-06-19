#pragma once

#include "CUnit.h"
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Core.h>

#include <string>
#include <utility>
#include <optional>
#include <vector>

namespace Perplex
{
	class Entity;
	class Scene;
	class ScriptProperty;
	class Simulator;

	struct ExternalFunction
	{
		std::string Name;
		void* Ptr;
	};

	class ScriptInstance
	{
	public:
		bool IsCompiled() { return m_Unit.IsCompiled(); }
		bool Compile(const std::string& src, Entity entity, const std::vector<ScriptProperty>& properties);

		template<typename ReturnType, typename... Args>
		std::optional<ReturnType> TryCall(const char* funcName, Args&&... funcArgs)
		{
			if (!m_Unit.IsCompiled())
				return std::nullopt;

			using FuncPtrType = ReturnType(*)(Args...);

			FuncPtrType funcPtr = reinterpret_cast<FuncPtrType>(m_Unit.GetSymbol(funcName));
			if (funcPtr != nullptr)
				return std::optional<ReturnType>{ funcPtr(std::forward<Args>(funcArgs)...) };

			return std::nullopt;
		}

		template<typename... Args>
		void TryCall(const char* funcName, Args&&... funcArgs)
		{
			if (!m_Unit.IsCompiled())
				return;

			using FuncPtrType = void(*)(Args...);

			FuncPtrType funcPtr = reinterpret_cast<FuncPtrType>(m_Unit.GetSymbol(funcName));
			if (funcPtr != nullptr)
				funcPtr(std::forward<Args>(funcArgs)...);
			return;
		}

		void AddFunction(const ExternalFunction& function);

		bool Started() const { return m_Started; }
		void SetStarted(bool val) { m_Started = val; }
	private:
		CUnit m_Unit{};
		UUID m_EntityID{ 0 };
		Scene* m_SceneContext{ nullptr };
		bool m_Started{ false };

		std::vector<ExternalFunction> m_ExternalFunctions = std::vector<ExternalFunction>();
		std::vector<ScriptProperty> m_Properties = std::vector<ScriptProperty>();
	};
}