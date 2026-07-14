#pragma once

#include <c/perplex_math.h>

#include <glm/fwd.hpp>
#include <nlohmann/json.hpp>

#define PERPLEX_JSON_TO(v1) nlohmann_json_j[#v1] = nlohmann_json_t.v1;
#define PERPLEX_JSON_FROM(v1) nlohmann_json_t.v1 = nlohmann_json_j.value(#v1, nlohmann_json_t.v1);

#define PERPLEX_DEFINE_JSON_STRUCT(Type, ...)  \
    template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int> = 0> \
    void to_json(BasicJsonType& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(PERPLEX_JSON_TO, __VA_ARGS__)) } \
    template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int> = 0> \
    void from_json(const BasicJsonType& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(PERPLEX_JSON_FROM, __VA_ARGS__)) }

namespace Perplex
{
	class JsonHelper
	{
	public:
		static void WriteToFile(const nlohmann::json& j, const std::filesystem::path& path);
		static nlohmann::json LoadFromFile(const std::filesystem::path& path);
	};
}

PERPLEX_DEFINE_JSON_STRUCT(Bounds, CenterX, CenterY, BoundsX, BoundsY)
PERPLEX_DEFINE_JSON_STRUCT(Radius, CenterX, CenterY, Radius)

namespace glm
{
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec2, x, y)
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec3, x, y, z)
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec4, r, g, b, a)
}
