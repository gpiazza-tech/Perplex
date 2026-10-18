#pragma once

#include <Perplex/Core/Application.h>
#include <Perplex/Angel/AngelEngine.h>
#include <Perplex/Parsing/Parser.h>

#include <angelscript.h>
#include <scriptarray/scriptarray.h>

namespace Perplex
{
	template<typename T>
	class AngelArray
	{
		AngelArray(size_t length = 0, const T& defaultValue = T{})
		{
			AngelEngine& engine = Application::Get().GetAngelEngine();
			std::string tTypeName = std::string{ GetTypeName<T>() };
			std::string typeName = "array<" + tTypeName + ">"; // e.g. "array<int>
			m_TypeInfo = engine.GetTypeInfoByName(typeName.c_str());
			m_Array = CScriptArray::Create(m_TypeInfo, (asUINT)length, (void*)&defaultValue);
		}

		size_t size() const { return m_Array->GetSize(); }

		T& at(size_t index) { return *(T*)m_Array->At(index); }
		const T& at(size_t index) const { return *(T*)m_Array->At(index); }

		T* data() { return (T*)m_Array->GetBuffer(); }
		const T* data() const { return (T*)m_Array->GetBuffer(); }

		void push_back(const T& val) { m_Array->InsertLast((void*)&val); }
		void insert(size_t at, const T& val) { m_Array->InsertAt(at, (void*)&val); }

		CScriptArray* array() { return m_Array; }

	private:
		CScriptArray* m_Array;
		asITypeInfo* m_TypeInfo;
	};
}