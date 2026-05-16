#pragma once

#include "CUnit.h"

#include <string>
#include <utility>
#include <optional>

namespace Holloware
{
	class Entity;
	class Scene;
	class ScriptProperty;

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
	private:
		CUnit m_Unit{};
		Scene* m_SceneContext = nullptr;
	};
}