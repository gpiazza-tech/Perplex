#pragma once

#include <string>
#include <utility>

struct TCCState;

namespace Holloware
{
	class Entity;
	class Scene;
	class ScriptProperty;

	void* GetCSymbol(TCCState* state, const char* name);

	class ScriptInstance
	{
	public:
		ScriptInstance() = default;
		ScriptInstance(const ScriptInstance& other)
		{
			m_SceneContext = other.m_SceneContext;
			m_State = nullptr; // important to prevent errors on destruction
		}
		~ScriptInstance();

		bool IsCompiled() { return m_State != nullptr; }
		bool Compile(const std::string& src, Entity entity);

		template<typename ReturnType, typename... Args>
		ReturnType TryCall(const char* funcName, Args&&... funcArgs)
		{
			if (m_State == nullptr)
				return ReturnType{};

			using FuncPtrType = ReturnType(*)(Args...);

			FuncPtrType funcPtr = reinterpret_cast<FuncPtrType>(GetCSymbol(m_State, funcName));
			if (funcPtr != nullptr)
				return funcPtr(std::forward<Args>(funcArgs)...);

			return ReturnType{};
		}
		template<typename... Args>
		void TryCall(const char* funcName, Args&&... funcArgs)
		{
			if (m_State == nullptr)
				return;

			using FuncPtrType = void(*)(Args...);

			FuncPtrType funcPtr = reinterpret_cast<FuncPtrType>(GetCSymbol(m_State, funcName));
			if (funcPtr != nullptr)
				funcPtr(std::forward<Args>(funcArgs)...);
			return;
		}
	private:
		TCCState* m_State = nullptr;
		Scene* m_SceneContext = nullptr;
	};
}