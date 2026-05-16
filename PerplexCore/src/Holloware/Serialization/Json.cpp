#include <pch.h>
#include "Json.h"

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <array>
#include <cstddef>

using JsonBytes = std::array<std::byte, 16>;

namespace Holloware
{
	static nlohmann::json& BytesToJson(JsonBytes bytes) { return reinterpret_cast<nlohmann::json&>(bytes); }
	static JsonBytes& JsonToBytes(nlohmann::json bytes) { return reinterpret_cast<JsonBytes&>(bytes); }

	Json::Json()
		: m_JsonBytes(JsonToBytes(nlohmann::json{}))
	{
		
	}

	Json::~Json()
	{

	}

	template<typename T>
	T Json::TryGet(const char* name)
	{
		return T{};
	}
}