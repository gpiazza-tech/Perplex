#pragma once

#include "CUnit.h"

#include <string>
#include <utility>
#include <optional>

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
		bool Compile(const std::string& src, Entity entity);

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
	private:
		CUnit m_Unit{};
		Simulator* m_SimulatorContext = nullptr;
		Scene* m_SceneContext = nullptr;

		std::vector<ExternalFunction> m_ExternalFunctions = std::vector<ExternalFunction>();
	};
}