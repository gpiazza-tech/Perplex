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
		bool Deserialize(T& value, const char* name) const
		{
			if (!m_Json.contains(name))
				return false;
			
			value = m_Json.value<T>();
			return true;
		}

		template<typename T>
		T Get(const char* name) const
		{
			if (!m_Json.contains(name))
				return T{};

			return m_Json.value<T>(name);
		}

		void WriteToFile(const std::filesystem::path& path);

		template<typename T>
		Json operator=(const T& other) noexcept { return m_Json = other; }
		Json operator[](const char* name) noexcept { return Json(m_Json[name]); }
	private:
		Json(nlohmann::json json) : m_Json(std::move(json)) {}
	private:
		nlohmann::json m_Json{};
	};
}