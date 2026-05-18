#pragma once

#include <Perplex/Core/PerplexTypes.h>

#include <nlohmann/json_fwd.hpp>

#include <any>
#include <sstream>
#include <string>

namespace Perplex
{
	template<typename T>
	static T StringToValue(const std::string& str)
	{
		std::stringstream ss;
		ss.str(str);
		T value;
		ss >> value;
		return value;
	}

	class ScriptProperty
	{
	public:
		ScriptProperty() = default;
		ScriptProperty(const std::string& name, const std::string& type, const std::string& strValue);

		const std::string& GetName() const { return m_Name; }
		const PerplexTypes GetType() const { return m_Type; }

		template<typename T>
		const T& GetValue() const { return std::any_cast<T>(m_Value); }
		void* GetPtr() { return &m_Value; }

		void TrySync(const ScriptProperty& property);

		void DrawGui();

		friend void to_json(nlohmann::json& j, const ScriptProperty& property);
		friend void from_json(const nlohmann::json& j, ScriptProperty& property);
	private:
		std::string m_Name{};
		PerplexTypes m_Type{};
		std::any m_Value{};
	};
}