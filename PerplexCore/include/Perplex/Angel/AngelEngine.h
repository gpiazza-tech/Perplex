#pragma once

#include <string>
#include <type_traits>
#include <bit>

class asIScriptEngine;
class asIScriptContext;
class asIScriptFunction;
struct asSFuncPtr;

namespace Perplex
{
	class AngelModule;

	class AngelEngine 
	{
	public:
		AngelEngine();
		~AngelEngine();

		AngelModule CreateModule(const std::string& name, const std::string& source);

		void Prepare(asIScriptFunction* func);
		void Execute();

		template<typename T> 
		void SetArg(int arg, T val)
		{
			if constexpr (!std::is_fundamental_v<T> && !std::is_pointer_v<T> && !std::is_reference_v<T>)
				SetArgObject(arg, &val);
			else if constexpr (std::is_reference_v<T> || std::is_pointer_v<T>)
				SetArgAddress(arg, val);
			else if constexpr (sizeof(T) == 1)
				SetArgByte(arg, std::bit_cast<uint8_t>(val));
			else if constexpr (sizeof(T) == 2)
				SetArgWord(arg, std::bit_cast<uint16_t>(val));
			else if constexpr (sizeof(T) == 4)
				SetArgDWord(arg, std::bit_cast<uint32_t>(val));
			else if constexpr (sizeof(T) == 8)
				SetArgQWord(arg, std::bit_cast<uint64_t>(val));
			else
				static_assert(false, "Could not resolve argument type!");
		}

		template<float>
		void SetArg(int arg, float val)
		{
			SetArgFloat(arg, val);
		}

		template<double>
		void SetArg(int arg, double val)
		{
			SetArgDouble(arg, val);
		}

		template<typename T>
		T GetReturn()
		{
			if constexpr (!std::is_fundamental_v<T> && !std::is_pointer_v<T> && !std::is_reference_v<T>)
				return *(T*)GetReturnObject();
			else if constexpr (std::is_reference_v<T> || std::is_pointer_v<T>)
				return (T)GetReturnAddress();
			else if constexpr (sizeof(T) == 1)
				return GetReturnByte();
			else if constexpr (sizeof(T) == 2)
				return GetReturnWord();
			else if constexpr (sizeof(T) == 4)
				return GetReturnDWord();
			else if constexpr (sizeof(T) == 8)
				return GetReturnQWord();
			else
				static_assert(false, "Could not resolve argument type!");
		}

		template<float>
		float GetReturn()
		{
			return GetReturnFloat();
		}

		template<double>
		double GetReturn()
		{
			return GetReturnDouble();
		}

		void SetObject(void* obj);

		asIScriptContext* GetContext() { return m_Context; }

		static AngelEngine& Get() { static AngelEngine engine; return engine; }
	private:
		void SetArgByte(int arg, uint8_t val);
		void SetArgWord(int arg, uint16_t val);
		void SetArgDWord(int arg, uint32_t val);
		void SetArgQWord(int arg, uint64_t val);
		void SetArgFloat(int arg, float val);
		void SetArgDouble(int arg, double val);
		void SetArgObject(int arg, void* obj);
		void SetArgAddress(int arg, void* addr);

		uint8_t  GetReturnByte();
		uint16_t GetReturnWord();
		uint32_t GetReturnDWord();
		uint64_t GetReturnQWord();
		float GetReturnFloat();
		double GetReturnDouble();
		void* GetReturnObject();
		void* GetReturnAddress();
	private:
		asIScriptEngine* m_Engine;
		asIScriptContext* m_Context;
	};
}