#pragma once

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <array>
#include <cstddef>
#include <utility>
#include <filesystem>
#include <optional>

namespace Perplex
{
	class Json
	{
	public:
		Json() = default;
		Json(const std::filesystem::path& pathToLoad);

		template<typename T>
		bool Deserialize(const char* name, T& value) const
		{
			if (!m_Json.contains(name))
			{
				HW_CORE_WARN("JSON: Key '{0}' not found, leaving value unchanged.", name);
			}

			else
			{
				value = m_Json[name].get<T>();
			}
		}

		template<typename T>
		bool Serialize(const char* name, T& value)
		{
			if (m_Json.contains(name))
			{
				HW_CORE_WARN("JSON: Key '{0}' already exists, overriding value.", name);
			}
			
			else
			{
				m_Json[name] = value;
			}
		}

		void WriteToFile(const std::filesystem::path& path);

		template<typename T>
		Json operator=(const T& other) noexcept { return m_Json = other; }
	private:
		Json(nlohmann::json json) : m_Json(std::move(json)) {}
	private:
		nlohmann::json m_Json{};
	};
}